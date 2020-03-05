#ifndef MICROUI_IMPL_SDL2_H
#define MICROUI_IMPL_SDL2_H

typedef struct SDL_Window SDL_Window;
typedef union SDL_Event SDL_Event;
typedef struct mu_Context mu_Context;

void microui_sdl2_init_opengl(mu_Context *ctx, SDL_Window *window, void *sdl_gl_context);
void microui_sdl2_shutdown();
void microui_sdl2_new_frame(SDL_Window *window);
void microui_sdl2_process_event(mu_Context *ctx, const SDL_Event *event);

#endif
