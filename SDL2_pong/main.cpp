/* SDL2_pong by iw0rm3r, 2023 */
/* Based on Lazy Foo' Productions code samples */

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
//#include <SDL_image.h>
#include <stdio.h>
//#include <string>

#include "g_base.h"
#include "g_logic.h"
#include "Dot.h"
#include "Racket.h"
#include "LTimer.h"

#include <dbg.h>

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Flags
			bool quit = false;
			bool showFramerate = true;

			//Event handler
			SDL_Event e;

			//Declaring objects
            Dot dot;
			Racket leftRacket( RACKET_X_BOUNDS_OFFSET, 0 );
			Racket rightRacket( SCREEN_WIDTH - RACKET_X_BOUNDS_OFFSET - RACKET_WIDTH, 1 );

			//Set the wall
//			SDL_Rect wall;
//			wall.x = 300;
//			wall.y = 40;
//			wall.w = 40;
//			wall.h = 400;

			//Define array of rects for dot to collide with
			SDL_Rect* objectRects[] { &leftRacket.mRect, &rightRacket.mRect };

            //Framerate calculations
            int countedFrames {0};
            LTimer fpsTimer;
            fpsTimer.start();

            //Keeps track of time between steps
            LTimer stepTimer;

            //Pre-render stuff
            renderScore();

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT ||
                        (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) ||
                        (e.type == SDL_JOYBUTTONDOWN && e.jbutton.which == 0
                            && e.jbutton.button == 7) )
					{
						quit = true;
                    //Framerate toggle
					} else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f ) {
                        showFramerate = !showFramerate;
					}

					//Handle input for the rackets
					leftRacket.handleEvent( e );
					rightRacket.handleEvent( e );
				}

                //Calculate time step in seconds
                float timeStep = stepTimer.getTicks() / 1000.f;

				//Move objects and check collision
				dot.move( timeStep, objectRects, ( sizeof(objectRects) /
                                                   sizeof(objectRects[0]) ) );
				leftRacket.move( timeStep );
				rightRacket.move( timeStep );

                //Restart step timer
                stepTimer.start();

                //Calculate framerate
                const int ticksPassed { static_cast<int>( fpsTimer.getTicks() ) };

                if( ticksPassed >= 1000 ) {
                    const int framerate { static_cast<int> ( (countedFrames /
                                                            (ticksPassed / 1000.0)) + 0.5 ) };
                    countedFrames = 0;
                    fpsTimer.start(); // reset the timer

                    //Render frame counter
                    if( showFramerate ) {
                        if( !gFramerateTexture.loadFromRenderedText( std::to_string(framerate),
                                                                    FOREGROUND_COLOR,
                                                                    gFramerateFont ) ) {
                            printf( "Unable to render FPS texture!\n" );
                        }
                    }
                }

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
				SDL_RenderClear( gRenderer );

				//Render wall
//				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//				SDL_RenderDrawRect( gRenderer, &wall );

				//Render objects
				dot.render();
				leftRacket.render();
				rightRacket.render();

				//Render frame counter and score
                if( showFramerate )
                    gFramerateTexture.render(2, 2);
                gScoreTexture.render( ( SCREEN_WIDTH / 2 ) - ( gScoreTexture.getWidth() / 2 ),
                                      SCORE_Y_POSITION );

				//Update screen
				SDL_RenderPresent( gRenderer );

				++countedFrames;
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
