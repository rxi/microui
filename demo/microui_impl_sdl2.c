#include "SDL2/SDL.h"
#include "microui.h"
#include "microui_impl_sdl2.h"
#include "atlas.h"


static const char button_map[256] = {
  [ SDL_BUTTON_LEFT   & 0xff ] =  MU_MOUSE_LEFT,
  [ SDL_BUTTON_RIGHT  & 0xff ] =  MU_MOUSE_RIGHT,
  [ SDL_BUTTON_MIDDLE & 0xff ] =  MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
  [ SDLK_LSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_RSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_LCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_RCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_LALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RETURN       & 0xff ] = MU_KEY_RETURN,
  [ SDLK_BACKSPACE    & 0xff ] = MU_KEY_BACKSPACE,
};


static void microui_sdl2_init(mu_Context *ctx, SDL_Window *window) {
  ctx->text_width = atlas_text_width;
  ctx->text_height = atlas_text_height;
}


void microui_sdl2_init_opengl(mu_Context *ctx, SDL_Window *window, void *sdl_gl_context) {
  /* no need to use it currently */
  (void)sdl_gl_context;

  microui_sdl2_init(ctx, window);
}


void microui_sdl2_shutdown() {
  // nothing to do
}


void microui_sdl2_new_frame(SDL_Window *window) {
  // nothing to do
}


void microui_sdl2_process_event(mu_Context *ctx, const SDL_Event *e) {
  switch (e->type) {
    case SDL_MOUSEMOTION: mu_input_mousemove(ctx, e->motion.x, e->motion.y); break;
    case SDL_MOUSEWHEEL: mu_input_scroll(ctx, 0, e->wheel.y * -30); break;
    case SDL_TEXTINPUT: mu_input_text(ctx, e->text.text); break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP: {
      int b = button_map[e->button.button & 0xff];
      if (b && e->type == SDL_MOUSEBUTTONDOWN) { mu_input_mousedown(ctx, e->button.x, e->button.y, b); }
      if (b && e->type ==   SDL_MOUSEBUTTONUP) { mu_input_mouseup(ctx, e->button.x, e->button.y, b);   }
      break;
    }

    case SDL_KEYDOWN:
    case SDL_KEYUP: {
      int c = key_map[e->key.keysym.sym & 0xff];
      if (c && e->type == SDL_KEYDOWN) { mu_input_keydown(ctx, c); }
      if (c && e->type ==   SDL_KEYUP) { mu_input_keyup(ctx, c);   }
      break;
    }
  }
}

