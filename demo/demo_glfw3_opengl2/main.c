#include <stdio.h>
#include <GLFW/glfw3.h>
#include "microui.h"
#include "microui_demo.h"
#include "microui_impl_opengl2.h"
#include "microui_impl_glfw3.h"


static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void process_frame(mu_Context *ctx) {
  mu_begin(ctx);
  style_window(ctx);
  log_window(ctx);
  test_window(ctx);
  mu_end(ctx);
}


int main(int argc, char **argv) {
  /* init GLFW */
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return -1;

  /* init GLFW window */
  GLFWwindow* window = glfwCreateWindow(800, 600, "", NULL, NULL);
  if (window == NULL)
    return -1;

  /* init OpenGL */
  glfwMakeContextCurrent(window);

  /* init microui */
  mu_Context _ctx;
  mu_Context *ctx = &_ctx;
  mu_init(ctx);

  /* setup platform/renderer bindings */
  microui_glfw3_init_opengl(ctx, window, 1);
  microui_opengl2_init();

  /* main loop */
  while (!glfwWindowShouldClose(window)) {
    /* poll events */
    glfwPollEvents();

    /* process frame */
    microui_glfw3_new_frame(window);
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    microui_opengl2_new_frame(w, h);
    process_frame(ctx);

    /* render */
    glClearColor(bg[0] / 255., bg[1] / 255., bg[2] / 255., 1.);
    glClear(GL_COLOR_BUFFER_BIT);
    microui_opengl2_render_data(ctx);
    glfwSwapBuffers(window);
  }

  microui_opengl2_shutdown();
  microui_glfw3_shutdown(window);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}


