#ifndef MICROUI_IMPL_SDL2_H
#define MICROUI_IMPL_SDL2_H

typedef struct GLFWwindow GLFWwindow;
typedef struct mu_Context mu_Context;

void microui_glfw3_init_opengl(mu_Context *ctx, GLFWwindow *window, int install_callbacks);
void microui_glfw3_shutdown(GLFWwindow *window);
void microui_glfw3_new_frame(GLFWwindow *window);

void microui_glfw3_CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
void microui_glfw3_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void microui_glfw3_ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void microui_glfw3_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void microui_glfw3_CharCallback(GLFWwindow *window, unsigned int c);


#endif
