#include "g_logic.h"

//Using SDL, SDL_image, standard IO, and strings
//#include <SDL_image.h>
#include <stdio.h>
//#include <string>

#include <dbg.h>

extern const int MAX_SCORE {10};
int gCurrentScore[2] {0, 0};

//Vector velocity of the dot (pixels per second)
extern const int DOT_MIN_VEL {300};
extern const int DOT_MAX_VEL {500};
extern const int DOT_VEL_DIFF { DOT_MAX_VEL - DOT_MIN_VEL };

void renderScore() {
    if( !gScoreTexture.loadFromRenderedText( std::to_string(gCurrentScore[0]) + ':' +
                                             std::to_string(gCurrentScore[1]),
                                             FOREGROUND_COLOR,
                                             gScoreFont ) ) {
        printf( "Unable to render score texture!\n" );
    }
}

void goal( bool leftPlayerScored, Dot* dot ) {
    if( leftPlayerScored ) {
        ++gCurrentScore[0];
        printf("LEFT player just scored! " );
    } else {
        ++gCurrentScore[1];
        printf("RIGHT player just scored! " );
    }

    if( gCurrentScore[0] == MAX_SCORE || gCurrentScore[1] == MAX_SCORE ) {
        printf("He won the game!\nStarting a new game now...\n\n");
        gCurrentScore[0] = 0;
        gCurrentScore[1] = 0;
    } else {
        printf("Score is %i:%i now\n", gCurrentScore[0], gCurrentScore[1]);
    }

    const int gameProgress = std::max( gCurrentScore[0], gCurrentScore[1] );
//    dbg(gameProgress);

    dot->vectorVelocity = DOT_MIN_VEL + ( gameProgress * DOT_VEL_DIFF / ( MAX_SCORE - 1 ) );
//    dbg(dot->vectorVelocity);
    dot->reset();
    renderScore();
}

//bool checkBump( float prevPos, float currPos, int prevVelosity, bool& bumpFlag ) {
//    if( prevPos == currPos ) {
//        if( prevVelosity != 0 && !bumpFlag ) {
//            bumpFlag = true;
//            return true;
//        }
//        return false;
//    } else {
//        bumpFlag = false;
//        return false;
//    }
//}

bool checkCollision( SDL_FRect a, SDL_Rect* b )
{
    //Calculate the sides of rect A
    const float leftA {a.x};
    const float rightA {a.x + a.w};
    const float topA {a.y};
    const float bottomA {a.y + a.h};

    //Calculate the sides of rect B
    const int leftB {b->x};
    const int rightB {b->x + b->w};
    const int topB {b->y};
    const int bottomB {b->y + b->h};

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
        return false;
    if( topA >= bottomB )
        return false;
    if( rightA <= leftB )
        return false;
    if( leftA >= rightB )
        return false;

    //If none of the sides from A are outside B - it's a collision
    return true;
}

Collision checkOutOfScreen( float position, int objSize, int screenSize )
{
    if ( position < 0 )
        return Collision::LeftTop;
    else if ( (position + objSize) > screenSize )
        return Collision::RightBottom;

    return Collision::None;
}

long map( long x, long in_min, long in_max, long out_min, long out_max )
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

