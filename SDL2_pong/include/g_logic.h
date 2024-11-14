/* All the game logic goes in here */

#ifndef G_LOGIC_H
#define G_LOGIC_H

#include <SDL.h>

#include "Dot.h"

// Constants declaration to avoid multiple definitions
extern const int MAX_SCORE;
extern int gCurrentScore[2];

//Vector velocity of the dot (pixels per second)
extern const int DOT_MIN_VEL;
extern const int DOT_MAX_VEL;
extern const int DOT_VEL_DIFF;

enum class Collision {None, LeftTop, RightBottom};

// Render texture with a score
void renderScore();

// Goal handler
void goal( bool leftPlayerScored, Dot* dot );

// Check if object just bumped the edge of the screen or another object
//bool checkBump( float prevPos, float currPos, int prevVelosity, bool& bumpFlag );

// Box collision detector
bool checkCollision( SDL_FRect a, SDL_Rect* b );

// Check if object went off the screen
Collision checkOutOfScreen( float position, int objSize, int screenSize );

// Map an integer from one range to another
long map( long x, long in_min, long in_max, long out_min, long out_max );

#endif // G_LOGIC_H
