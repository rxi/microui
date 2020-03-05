#include "GLFW/glfw3.h"
#include "microui.h"
#include "microui_impl_glfw3.h"
#include "atlas.h"

static mu_Context *g_ctx = NULL;

static const char button_map[256] = {
  [ GLFW_MOUSE_BUTTON_LEFT   & 0xff ] =  MU_MOUSE_LEFT,
  [ GLFW_MOUSE_BUTTON_RIGHT  & 0xff ] =  MU_MOUSE_RIGHT,
  [ GLFW_MOUSE_BUTTON_MIDDLE & 0xff ] =  MU_MOUSE_MIDDLE,
};

static const char key_map[512] = {
  [ GLFW_KEY_LEFT_SHIFT     & 0x1ff ] = MU_KEY_SHIFT,
  [ GLFW_KEY_RIGHT_SHIFT    & 0x1ff ] = MU_KEY_SHIFT,
  [ GLFW_KEY_LEFT_CONTROL   & 0x1ff ] = MU_KEY_CTRL,
  [ GLFW_KEY_RIGHT_CONTROL  & 0x1ff ] = MU_KEY_CTRL,
  [ GLFW_KEY_LEFT_ALT       & 0x1ff ] = MU_KEY_ALT,
  [ GLFW_KEY_RIGHT_ALT      & 0x1ff ] = MU_KEY_ALT,
  [ GLFW_KEY_ENTER          & 0x1ff ] = MU_KEY_RETURN,
  [ GLFW_KEY_BACKSPACE      & 0x1ff ] = MU_KEY_BACKSPACE,
};

static GLFWcursorposfun   user_CursorPosCallback   = NULL;
static GLFWmousebuttonfun user_MousebuttonCallback = NULL;
static GLFWscrollfun      user_ScrollCallback      = NULL;
static GLFWkeyfun         user_KeyCallback         = NULL;
static GLFWcharfun        user_CharCallback        = NULL;
static int installed_callbacks = GLFW_FALSE;


static void microui_glfw3_init(mu_Context *ctx, GLFWwindow *window, int install_callbacks) {
  ctx->text_width = atlas_text_width;
  ctx->text_height = atlas_text_height;

  if (install_callbacks) {
    user_CursorPosCallback   = glfwSetCursorPosCallback(window, microui_glfw3_CursorPosCallback);
    user_MousebuttonCallback = glfwSetMouseButtonCallback(window, microui_glfw3_MouseButtonCallback);
    user_ScrollCallback      = glfwSetScrollCallback(window, microui_glfw3_ScrollCallback);
    user_KeyCallback         = glfwSetKeyCallback(window, microui_glfw3_KeyCallback);
    user_CharCallback        = glfwSetCharCallback(window, microui_glfw3_CharCallback);
    installed_callbacks      = GLFW_TRUE;
  }

  g_ctx = ctx;
}


void microui_glfw3_init_opengl(mu_Context *ctx, GLFWwindow *window, int install_callbacks) {
  microui_glfw3_init(ctx, window, install_callbacks);
}


void microui_glfw3_shutdown(GLFWwindow *window) {
  if (installed_callbacks) {
    glfwSetCursorPosCallback(window, user_CursorPosCallback);
    glfwSetMouseButtonCallback(window, user_MousebuttonCallback);
    glfwSetScrollCallback(window, user_ScrollCallback);
    glfwSetKeyCallback(window, user_KeyCallback);
    glfwSetCharCallback(window, user_CharCallback);
    user_CursorPosCallback   = NULL;
    user_MousebuttonCallback = NULL;
    user_ScrollCallback      = NULL;
    user_KeyCallback         = NULL;
    user_CharCallback        = NULL;
    installed_callbacks      = GLFW_FALSE;
  }

  g_ctx = NULL;
}


void microui_glfw3_new_frame(GLFWwindow *window) {
  // nothing to do
}


void microui_glfw3_CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  if (user_CursorPosCallback) {
    user_CursorPosCallback(window, xpos, ypos);
  }

  mu_input_mousemove(g_ctx, xpos, ypos);
}


void microui_glfw3_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (user_MousebuttonCallback) {
    user_MousebuttonCallback(window, button, action, mods);
  }

  int b = button_map[button & 0xff];
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (b && action ==   GLFW_PRESS) { mu_input_mousedown(g_ctx, xpos, ypos, b); }
  if (b && action == GLFW_RELEASE) { mu_input_mouseup(g_ctx, xpos, ypos, b); }
}


void microui_glfw3_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  if (user_ScrollCallback) {
    user_ScrollCallback(window, xoffset, yoffset);
  }

  mu_input_scroll(g_ctx, xoffset, yoffset * -30);
}


void microui_glfw3_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (user_KeyCallback) {
    user_KeyCallback(window, key, scancode, action, mods);
  }

  int c = key_map[key & 0x1ff];
  if (c && action ==   GLFW_PRESS) { mu_input_keydown(g_ctx, c); }
  if (c && action == GLFW_RELEASE) { mu_input_keyup(g_ctx, c);   }
}


void microui_glfw3_CharCallback(GLFWwindow* window, unsigned int c) {
  if (user_CharCallback) {
    user_CharCallback(window, c);
  }

  if (c > 127) {
    /* not valid ascii charactor */
    mu_input_text(g_ctx, "?");
  } else {
    char ascii_c[2] = {(char)c, '\0'};
    mu_input_text(g_ctx, ascii_c);
  }
}
