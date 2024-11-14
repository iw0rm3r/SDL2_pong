#include "Dot.h"

//Using SDL, SDL_image, standard IO, and strings
//#include <SDL_image.h>
//#include <stdio.h>
//#include <string>
#include <cmath>

#include "g_logic.h"

#include <dbg.h>

Dot::Dot()
{
	//Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

    //Initialize vector velocity
    vectorVelocity = DOT_MIN_VEL;

	reset();
}

void Dot::move( float timeStep, SDL_Rect* objectRects[], int objNumber )
{
//    printf( "X velocity is %i, Y veloticy if %i\n", mVelX, mVelY );

    // Exit if nothing to do
    if ( mVelX == 0 && mVelY == 0) {
//        storeVelocity();
        return;
    }

    // Store previous coordinates to compare function results
//    const float prevPosX { mPosX };
//    const float prevPosY { mPosY };

    // Move the dot both axes
    mPosX += mVelX * timeStep;
    mPosY += mVelY * timeStep;

//    printf( "X is %f, Y is %f\n", mPosX, mPosY );

    // Goal detection
    const Collision offTheScreen { checkOutOfScreen(mPosX, DOT_WIDTH, SCREEN_WIDTH) };

    if ( offTheScreen == Collision::LeftTop ) {
        goal( false, this );
        return;
    } else if ( offTheScreen == Collision::RightBottom ) {
        goal( true, this );
        return;
    }

    // If dot went off screen - return it back
    bool wentOutOfScreen { outOfScreen(mPosY, mVelY, DOT_HEIGHT, SCREEN_HEIGHT) };

    bool dotCollided {false};
    if ( !wentOutOfScreen ) {
        // Check collisions
        mCollider.x = mPosX;
        mCollider.y = mPosY;

        for(int i = 0; i < objNumber; ++i) {
            bool collision { checkCollision(mCollider, objectRects[i]) };
            if( collision ) {
    //            printf("Collision detected!\n");
                // Detect collision side, move dot to the object and inverse velocity
                if ( mVelY == 0 ) {         // horizontal movement
                    moveCollided( mVelX, mPosX, DOT_WIDTH, objectRects[i]->x, objectRects[i]->w );
                    hitDot( mPosY, objectRects[i]->y, i );
                } else if (mVelX == 0) {    // vertical movement
                    moveCollided( mVelY, mPosY, DOT_HEIGHT, objectRects[i]->y, objectRects[i]->h );
                } else {                    // diagonal movement
                    // move X position back and check if it will solve collision
                    mCollider.x = mPosX - (mVelX * timeStep);
                    if ( !checkCollision(mCollider, objectRects[i]) ) {
                        moveCollided( mVelX, mPosX, DOT_WIDTH, objectRects[i]->x, objectRects[i]->w );
                        hitDot( mPosY, objectRects[i]->y, i );
                    } else {
                        moveCollided( mVelY, mPosY, DOT_HEIGHT, objectRects[i]->y, objectRects[i]->h );
                    }
                }
            }
            dotCollided |= collision;
        }
    }

    // Compare dot position before and after movement to make a bump
//    bool bump { false };
//
//    if ( mVelX != 0 && mVelY != 0 ) { // diagonal movement
//        bump |= checkBump( prevPosX, mPosX, prevVelX, xAxisBumped );
//        bump |= checkBump( prevPosY, mPosY, prevVelY, yAxisBumped );
//    } else if ( mVelX != 0 ) {
//        bump = checkBump( prevPosX, mPosX, prevVelX, xAxisBumped );
//    } else {
//        bump = checkBump( prevPosY, mPosY, prevVelY, yAxisBumped );
//    }
//
//    if ( bump )
//        doBump();
//
//    storeVelocity();
    if( wentOutOfScreen || dotCollided )
        doBump();
}

void Dot::hitDot( float dotPos, int racketPos, int objIndex ) {
    // Exit if object is not a racket
    if( objIndex  > 1 ) return;

    // Calculate hit point
    const float hitPointY { ( racketPos + (RACKET_HEIGHT / 2) ) -
                            ( dotPos + (DOT_HEIGHT / 2) ) };
//    dbg(hitPointY);

    if( objIndex == 0 ) { // left racket
        if( hitPointY <= 0 ) { // up
            vectorAngle =       (  hitPointY * RACKET_HIT_MAX_ANGLE ) / RACKET_HIT_Y_RANGE;
        } else {               // down
            vectorAngle = 360 - ( -hitPointY * RACKET_HIT_MAX_ANGLE ) / RACKET_HIT_Y_RANGE;
        }
    } else {              // right racket
        if( hitPointY <= 0 ) { // up
            vectorAngle = 180 - (  hitPointY * RACKET_HIT_MAX_ANGLE ) / RACKET_HIT_Y_RANGE;
        } else {               // down
            vectorAngle = 180 + ( -hitPointY * RACKET_HIT_MAX_ANGLE ) / RACKET_HIT_Y_RANGE;
        }
    }
//    dbg(vectorAngle);
    calcAxisVel();
}

void Dot::doBump() {
//    dbg( dbg::time(), "Bump!\n" );

    for( int jIdx = 0; jIdx < 2; ++jIdx ) {
        if( gJoyHaptic[jIdx] != NULL ) {
            // Play rumble at 25% strength for 50 milliseconds
            if( SDL_HapticRumblePlay( gJoyHaptic[jIdx], 0.75, 50 ) != 0 ) {
                printf( "Warning: Unable to play haptic rumble with joystick #%i! %s\n", (jIdx + 1), SDL_GetError() );
            }
        }
    }
}

//void Dot::storeVelocity() {
//    prevVelX = mVelX;
//    prevVelY = mVelY;
//}

void Dot::moveCollided( float& velocity, float& aPos, int aSize, int bPos, int bSize ) {
    if ( velocity > 0 ) aPos = bPos - aSize; // from left/top
    else                aPos = bPos + bSize; // from right/bottom

    velocity = -velocity;
}

bool Dot::outOfScreen( float& position, float& velocity, int objSize, int screenSize )
{
    const Collision offTheScreen{ checkOutOfScreen(position, objSize, screenSize) };

    if ( offTheScreen == Collision::LeftTop ) {
        position = 0;
        velocity = -velocity;
        return true;
    } else if ( offTheScreen == Collision::RightBottom ) {
        position = screenSize - objSize;
        velocity = -velocity;
        return true;
    }

    return false;
}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( static_cast<int>( mPosX ), static_cast<int>( mPosY ) );
}

void Dot::reset() {
    //Initialize the offsets
    mPosX = DOT_START_X;
    mPosY = DOT_START_Y;

    //Initialize the velocity (aim at 90 degrees range to left or right)
    vectorAngle = ( rand() % 180 );
    if( vectorAngle >= 45 ) {
        vectorAngle += 90;
        if ( vectorAngle >= 225 ) {
            vectorAngle += 90;
        }
    }
//    dbg(startAngle);
    calcAxisVel();

//    prevVelX = 0;
//    prevVelY = 0;
//
//	xAxisBumped = false;
//	yAxisBumped = false;
}

void Dot::calcAxisVel() {
    const float vectorAngleRads = vectorAngle * PI / 180; // in radians

    mVelX =    vectorVelocity * cos(vectorAngleRads);
    mVelY = -( vectorVelocity * sin(vectorAngleRads) );
}
