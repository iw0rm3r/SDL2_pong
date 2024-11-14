#ifndef DOT_H
#define DOT_H

#include <SDL.h>

#include "LTexture.h"
#include "g_base.h"

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

        //Vector properties
        int vectorVelocity;
        int vectorAngle; // in degrees

		//Initializes the variables
		Dot();

		//Moves the dot and checks collision
		void move( float timeStep, SDL_Rect* objectRects[], int objNumber );

		//Hit the dot off the racket
        void hitDot( float dotPos, int racketPos, int objIndex );

        //Do a bump
        void doBump();

		//Store previous velocity to compare function results and make a bump
//        void storeVelocity();

		//Detect collision side and move dot to the object
		void moveCollided( float& velocity, float& aPos, int aSize, int bPos, int bSize );

		//Check if dot went off screen and return it
        bool outOfScreen( float& position, float& velocity, int objSize, int screenSize );

		//Shows the dot on the screen
		void render();

		//Reset properties of the dot
		void reset();

		//Convert vector to axis velocity
        void calcAxisVel();

    private:
		//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		float mVelX, mVelY;

        const int DOT_START_X { (SCREEN_WIDTH / 2) - (DOT_WIDTH) };
        const int DOT_START_Y { (SCREEN_HEIGHT / 2) - (DOT_HEIGHT / 2) };

		//The velocity of the dot
//		float prevVelX, prevVelY;

		//Collision flags from the previous frame
//		bool xAxisBumped, yAxisBumped;

		//Dot's collision box (floating point version)
		SDL_FRect mCollider;
};

#endif // DOT_H
