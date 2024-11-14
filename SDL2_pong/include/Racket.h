#ifndef RACKET_H
#define RACKET_H

#include <SDL.h>

//The racket that will bounce off the dot
class Racket
{
    public:
		//Maximum axis velocity of the racket (pixels per second)
		static const int RACKET_VEL = 640;

		//Racket's rectangle to draw and check collisions
		SDL_Rect mRect;

		//Initializes the variables
		Racket( int xPosition, int racketIndex );

		//Takes key presses and adjusts the racket's velocity
		void handleEvent( SDL_Event& e );

		//Moves the racket and checks collision
		void move( float timeStep );

		//Shows the racket on the screen
		void render();

    private:
		//The X and Y offsets of the racket
		float mPosX, mPosY;

		//The velocity of the racket
		int mVelY;

		//Keys to control the racket
        int keyUp, keyDown;

        //Joystick to recieve commands from
        int joystickIndex;
};

#endif // RACKET_H
