/* Basic application init-dealloc stuff and app-wide variables */

#ifndef G_BASE_H
#define G_BASE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

// Forward declaration to avoid cross-inclusion
class LTexture;

// Constants declaration to avoid multiple definitions
extern const float PI;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int JOY_DEADZONE;
extern const int JOY_MAX_AXIS_VAL;
extern const int JOY_MIN_AXIS_VAL;
extern const SDL_Color FOREGROUND_COLOR;
extern const SDL_Color BACKGROUND_COLOR;

extern const int RACKET_WIDTH;
extern const int RACKET_HEIGHT;
extern const int RACKET_HIT_Y_RANGE;
extern const int RACKET_HIT_MAX_ANGLE;
extern const int RACKET_START_Y_POS;

extern const int RACKET_X_BOUNDS_OFFSET;
extern const int RACKET_Y_BOUNDS_OFFSET;
extern const int RACKET_MIN_Y_POS;
extern const int RACKET_MAX_Y_POS;

extern const int SCORE_Y_POSITION;

extern const std::string APP_NAME;
extern const std::string RESOURCE_DIR;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Game controller 1 and haptic handlers
extern SDL_Joystick* gGameController[2];
extern SDL_Haptic* gJoyHaptic[2];

//Globally used font
extern TTF_Font* gFramerateFont;
extern TTF_Font* gScoreFont;

//Scene textures
extern LTexture gDotTexture;
extern LTexture gFramerateTexture;
extern LTexture gScoreTexture;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

#endif // G_BASE_H
