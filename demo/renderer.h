#ifndef RENDERER_H
#define RENDERER_H

#include "microui.h"

#ifdef SDL3_RENDERER
#  include "sdl3-renderer.h"
#else
#  include "sdl2-renderer.h"
#endif

#define r_PollEvent(E) SDL_PollEvent(E)
typedef SDL_Event      r_Event;

static const char r_button_map[256] = {
  [ SDL_BUTTON_LEFT   & 0xff ] =  MU_MOUSE_LEFT,
  [ SDL_BUTTON_RIGHT  & 0xff ] =  MU_MOUSE_RIGHT,
  [ SDL_BUTTON_MIDDLE & 0xff ] =  MU_MOUSE_MIDDLE,
};


void r_init(void);
void r_draw_rect(mu_Rect rect, mu_Color color);
void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color);
void r_draw_icon(int id, mu_Rect rect, mu_Color color);
 int r_get_event_key_modifier(r_Event event);
 int r_get_text_width(const char *text, int len);
 int r_get_text_height(void);
void r_set_clip_rect(mu_Rect rect);
void r_clear(mu_Color color);
void r_present(void);


#endif

