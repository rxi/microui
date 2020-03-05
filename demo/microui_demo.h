#ifndef MICROUI_DEMO_H
#define MICROUI_DEMO_H

typedef struct mu_Context mu_Context;

void style_window(mu_Context *ctx);
void log_window(mu_Context *ctx);
void test_window(mu_Context *ctx);

extern float bg[3];

#endif
