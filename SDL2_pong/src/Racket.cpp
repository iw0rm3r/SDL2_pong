#include "Racket.h"

//Using SDL, SDL_image, standard IO, and strings
//#include <SDL_image.h>
//#include <stdio.h>
//#include <string>

#include "g_logic.h"
#include "g_base.h"

#include <dbg.h>

Racket::Racket( int xPosition, int racketIndex )
{
    //Initialize the offsets
    mPosX = xPosition;
    mPosY = RACKET_START_Y_POS;

	//Set rect dimension and position
	mRect.x = mPosX;
	mRect.y = mPosY;
	mRect.w = RACKET_WIDTH;
	mRect.h = RACKET_HEIGHT;

    //Initialize the velocity
    mVelY = 0;

    if( racketIndex == 0 ) { // if it's a left racket
        keyUp = SDLK_a;
        keyDown = SDLK_z;
        joystickIndex = 0;
    } else {           // if it's a right racket
        keyUp = SDLK_UP;
        keyDown = SDLK_DOWN;
        joystickIndex = 1;
    }
}

void Racket::handleEvent( SDL_Event& e )
{
    // If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        // Adjust the velocity
        if     ( e.key.keysym.sym == keyUp )
            mVelY -= RACKET_VEL;
        else if( e.key.keysym.sym == keyDown )
            mVelY += RACKET_VEL;
    }
    // If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        // Adjust the velocity
        if     ( e.key.keysym.sym == keyUp )
            mVelY += RACKET_VEL;
        else if( e.key.keysym.sym == keyDown )
            mVelY -= RACKET_VEL;
    }
    // Joystick POV hat movement
    else if( e.type == SDL_JOYHATMOTION && e.jhat.which == joystickIndex )
    {
        // Adjust the velocity
        switch( e.jhat.value )
        {
            case SDL_HAT_UP: mVelY = -RACKET_VEL; break;
            case SDL_HAT_DOWN: mVelY = RACKET_VEL; break;
            case SDL_HAT_CENTERED: mVelY = 0; break;
        }
    }
    // Left joystick (of gamepad) movement
    else if( e.type == SDL_JOYAXISMOTION && e.jaxis.which == joystickIndex ) {
        if( e.jaxis.axis == 1 ) { // Y axis motion
            if( e.jaxis.value <= -JOY_DEADZONE )
                mVelY = map( e.jaxis.value, -JOY_DEADZONE, JOY_MIN_AXIS_VAL, -1, -RACKET_VEL );
            else if( e.jaxis.value >= JOY_DEADZONE )
                mVelY = map( e.jaxis.value, JOY_DEADZONE, JOY_MAX_AXIS_VAL, 1, RACKET_VEL );
            else
                mVelY = 0;
        }
    }
//    printf( "mVelY is %f\n", mVelY);
}

void Racket::move( float timeStep )
{
    // Exit if nothing to do
    if ( mVelY == 0)
        return;

//    printf( "Y veloticy if %i\n", mVelY );

    // Move the racket
    mPosY += mVelY * timeStep;

//    printf( "X is %f, Y is %f\n", mPosX, mPosY );

    // If racket went out of bounds - return it back
    if( mPosY < RACKET_MIN_Y_POS )
        mPosY = RACKET_MIN_Y_POS;
    else if( mPosY > RACKET_MAX_Y_POS )
        mPosY = RACKET_MAX_Y_POS;

    // Assign coordinates to SDL_Rect
	mRect.y = mPosY;
}

void Racket::render()
{
    // Show the racket
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRect( gRenderer, &mRect );
}
