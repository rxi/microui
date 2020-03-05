#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include "microui.h"
#include "microui_demo.h"
#include "microui_impl_opengl2.h"
#include "microui_impl_sdl2.h"


static void process_frame(mu_Context *ctx) {
  mu_begin(ctx);
  style_window(ctx);
  log_window(ctx);
  test_window(ctx);
  mu_end(ctx);
}

static void SDL_check(int should_true) {
  if (!should_true) {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}


int main(int argc, char **argv) {
  /* init SDL */
  SDL_check(SDL_Init(SDL_INIT_EVERYTHING) == 0);

  /* init SDL window */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow(
    NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_check(window != NULL);

  /* init OpenGL */
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  SDL_check(glcontext != NULL);

  /* init microui */
  mu_Context _ctx;
  mu_Context *ctx = &_ctx;
  mu_init(ctx);

  /* setup platform/renderer bindings */
  microui_sdl2_init_opengl(ctx, window, glcontext);
  microui_opengl2_init();

  /* main loop */
  for (;;) {
    /* handle SDL events */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      microui_sdl2_process_event(ctx, &e);
      if (e.type == SDL_QUIT) {
        goto loopend;
      }
    }

    /* process frame */
    microui_sdl2_new_frame(window);
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    microui_opengl2_new_frame(w, h);
    process_frame(ctx);

    /* render */
    glClearColor(bg[0] / 255., bg[1] / 255., bg[2] / 255., 1.);
    glClear(GL_COLOR_BUFFER_BIT);
    microui_opengl2_render_data(ctx);
    SDL_GL_SwapWindow(window);
  }
loopend:
  microui_opengl2_shutdown();
  microui_sdl2_shutdown();
  return 0;
}


