# SDL2_pong
**Simple and plain Pong game based on SDL2**

![SDL2_pong](https://raw.githubusercontent.com/iw0rm3r/SDL2_pong/main/screenshot.png)

2-player Pong game made as an example of SDL2 programming. Goal is hardcoded (`MAX_SCORE`, 10). Controls are:
 - A/Z for left racket movement
 - up/down for right racket movement
 - F to toggle framerate counter
 - ESC to exit the game
DirectInput joysticks/gamepads support is also implemented: gamepad 1 is left player, and second one is right. Vibration feedback is present.

**Requirements:**
SDL2 libraries: https://github.com/orgs/libsdl-org/repositories
Linker options list to build this project using MinGW32 is:
`-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf`
Really helpful tutorial on how to start with SDL2 by Lazy Foo: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php
