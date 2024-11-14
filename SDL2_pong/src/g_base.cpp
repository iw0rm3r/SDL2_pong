#include "g_base.h"

//Using SDL, SDL_image, standard IO, and strings
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>

//Inclusion of headers in .cpp file to avoid cross-inclusion
#include "LTexture.h"
#include "Dot.h"

extern const float PI {3.14159265};

extern const int SCREEN_WIDTH {640};
extern const int SCREEN_HEIGHT {480};
extern const int JOY_DEADZONE {8000}; // 8000 by default
extern const int JOY_MAX_AXIS_VAL {32767};
extern const int JOY_MIN_AXIS_VAL {-32768};
extern const SDL_Color FOREGROUND_COLOR { 0xFF, 0xFF, 0xFF };
extern const SDL_Color BACKGROUND_COLOR { 0, 0, 0 };

extern const int RACKET_WIDTH {5};
extern const int RACKET_HEIGHT {60};
extern const int RACKET_HIT_Y_RANGE { ( RACKET_HEIGHT / 2 ) + ( Dot::DOT_HEIGHT / 2 ) };
extern const int RACKET_HIT_MAX_ANGLE {45};
extern const int RACKET_START_Y_POS { (SCREEN_HEIGHT / 2) - (RACKET_HEIGHT / 2) };

extern const int RACKET_X_BOUNDS_OFFSET { static_cast<int>( SCREEN_WIDTH * 0.05 ) };
extern const int RACKET_Y_BOUNDS_OFFSET {20};
extern const int RACKET_MIN_Y_POS { RACKET_Y_BOUNDS_OFFSET };
extern const int RACKET_MAX_Y_POS { SCREEN_HEIGHT - RACKET_Y_BOUNDS_OFFSET
                                    - RACKET_HEIGHT };

extern const int SCORE_Y_POSITION { static_cast<int>( SCREEN_HEIGHT * 0.05 ) };

extern const std::string APP_NAME {"SDL2_pong"};
extern const std::string RESOURCE_DIR {"resources"};
static const std::string FONT_MONO_NAME {"digital-7 (mono).ttf"};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Game controller handlers
SDL_Joystick* gGameController[2] { NULL, NULL };
SDL_Haptic* gJoyHaptic[2] { NULL, NULL };

//Globally used font
TTF_Font* gFramerateFont = NULL;
TTF_Font* gScoreFont = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gFramerateTexture;
LTexture gScoreTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        //Check for joysticks
        if( SDL_NumJoysticks() < 1 )
        {
            printf( "Warning: No joysticks connected!\n" );
        }
        else
        {
            //Load joysticks
            for( int jIdx = 0; jIdx < 2; ++jIdx ) {
                gGameController[jIdx] = SDL_JoystickOpen( jIdx );
                if( gGameController[jIdx] == NULL )
                {
                    printf( "Warning: Unable to open game controller #%i! SDL Error: %s\n", (jIdx + 1), SDL_GetError() );
                }
                else
                {
                    //Check if joystick supports haptic
                    if( !SDL_JoystickIsHaptic( gGameController[jIdx] ) )
                    {
                        printf( "Warning: Controller #%i does not support haptics! SDL Error: %s\n", (jIdx + 1), SDL_GetError() );
                    }
                    else
                    {
                        //Get joystick haptic device
                        gJoyHaptic[jIdx] = SDL_HapticOpenFromJoystick( gGameController[jIdx] );
                        if( gJoyHaptic[jIdx] == NULL )
                        {
                            printf( "Warning: Unable to get joystick #%i haptics! SDL Error: %s\n", (jIdx + 1), SDL_GetError() );
                        }
                        else
                        {
                            //Initialize rumble
                            if( SDL_HapticRumbleInit( gJoyHaptic[jIdx] ) < 0 )
                            {
                                printf( "Warning: Unable to initialize joystick #%i haptic rumble! SDL Error: %s\n", (jIdx + 1), SDL_GetError() );
                            }
                        }
                    }
                }
            }
        }

		//Create window
		gWindow = SDL_CreateWindow( APP_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED
                                  | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n",
                        IMG_GetError() );
					success = false;
				}

                 //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
			}
		}
	}

	// Initialize a random number generator
	srand( static_cast<unsigned>( time(NULL) ) );

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    //Open fonts
    gFramerateFont = TTF_OpenFont( (RESOURCE_DIR + '/' + FONT_MONO_NAME).c_str(), 20 );
    if( gFramerateFont == NULL )
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    gScoreFont = TTF_OpenFont( (RESOURCE_DIR + '/' + FONT_MONO_NAME).c_str(), 36 );
    if( gScoreFont == NULL )
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

	//Load press texture
	if( !gDotTexture.loadFromFile( RESOURCE_DIR + "/dot.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
    gFramerateTexture.free();
    gScoreTexture.free();

    //Close game controllers with haptics
    for( int jIdx = 0; jIdx < 2; ++jIdx ) {
        if( gJoyHaptic[jIdx] != NULL ) {
            SDL_HapticClose( gJoyHaptic[jIdx] );
            gJoyHaptic[jIdx] = NULL;
        }
        if( gGameController[jIdx] != NULL ) {
            SDL_JoystickClose( gGameController[jIdx] );
            gGameController[jIdx] = NULL;
        }
    }

    //Free fonts
    TTF_CloseFont( gFramerateFont );
    gFramerateFont = NULL;
    TTF_CloseFont( gScoreFont );
    gScoreFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
