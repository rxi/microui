#include <SDL2/SDL.h>

#define R_EV_QUIT            SDL_QUIT
#define R_EV_KEYUP           SDL_KEYUP
#define R_EV_KEYDOWN         SDL_KEYDOWN
#define R_EV_MOUSEMOTION     SDL_MOUSEMOTION
#define R_EV_MOUSEBUTTONUP   SDL_MOUSEBUTTONUP
#define R_EV_MOUSEBUTTONDOWN SDL_MOUSEBUTTONDOWN
#define R_EV_MOUSEWHEEL      SDL_MOUSEWHEEL
#define R_EV_TEXTINPUT       SDL_TEXTINPUT

static const char r_key_map[256] = {
  [ SDLK_LSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_RSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_LCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_RCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_LALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RETURN       & 0xff ] = MU_KEY_RETURN,
  [ SDLK_BACKSPACE    & 0xff ] = MU_KEY_BACKSPACE,
 };
