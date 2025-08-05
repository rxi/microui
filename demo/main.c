#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "microui.h"


static  char logbuf[64000];
static   int logbuf_updated = 0;
static float bg[3] = { 90, 95, 100 };


static void write_log(const char *text) {
  if (logbuf[0]) { strcat(logbuf, "\n"); }
  strcat(logbuf, text);
  logbuf_updated = 1;
}


static void test_window(mu_Context *ctx) {
  /* do window */
  if (mu_begin_window(ctx, "Demo Window", mu_rect(40, 40, 300, 450))) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 240);
    win->rect.h = mu_max(win->rect.h, 300);

    /* window info */
    if (mu_header(ctx, "Window Info")) {
      mu_Container *win = mu_get_current_container(ctx);
      char buf[64];
      mu_layout_row(ctx, 2, (int[]) { 54, -1 }, 0);
      mu_label(ctx,"Position:");
      sprintf(buf, "%d, %d", win->rect.x, win->rect.y); mu_label(ctx, buf);
      mu_label(ctx, "Size:");
      sprintf(buf, "%d, %d", win->rect.w, win->rect.h); mu_label(ctx, buf);
    }

    /* labels + buttons */
    if (mu_header_ex(ctx, "Test Buttons", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
      mu_label(ctx, "Test buttons 1:");
      if (mu_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
      if (mu_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
      mu_label(ctx, "Test buttons 2:");
      if (mu_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
      if (mu_button(ctx, "Popup")) { mu_open_popup(ctx, "Test Popup"); }
      if (mu_begin_popup(ctx, "Test Popup")) {
        mu_button(ctx, "Hello");
        mu_button(ctx, "World");
        mu_end_popup(ctx);
      }
    }

    /* tree */
    if (mu_header_ex(ctx, "Tree and Text", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 2, (int[]) { 140, -1 }, 0);
      mu_layout_begin_column(ctx);
      if (mu_begin_treenode(ctx, "Test 1")) {
        if (mu_begin_treenode(ctx, "Test 1a")) {
          mu_label(ctx, "Hello");
          mu_label(ctx, "world");
          mu_end_treenode(ctx);
        }
        if (mu_begin_treenode(ctx, "Test 1b")) {
          if (mu_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
          if (mu_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
          mu_end_treenode(ctx);
        }
        mu_end_treenode(ctx);
      }
      if (mu_begin_treenode(ctx, "Test 2")) {
        mu_layout_row(ctx, 2, (int[]) { 54, 54 }, 0);
        if (mu_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
        if (mu_button(ctx, "Button 4")) { write_log("Pressed button 4"); }
        if (mu_button(ctx, "Button 5")) { write_log("Pressed button 5"); }
        if (mu_button(ctx, "Button 6")) { write_log("Pressed button 6"); }
        mu_end_treenode(ctx);
      }
      if (mu_begin_treenode(ctx, "Test 3")) {
        static int checks[3] = { 1, 0, 1 };
        mu_checkbox(ctx, "Checkbox 1", &checks[0]);
        mu_checkbox(ctx, "Checkbox 2", &checks[1]);
        mu_checkbox(ctx, "Checkbox 3", &checks[2]);
        mu_end_treenode(ctx);
      }
      mu_layout_end_column(ctx);

      mu_layout_begin_column(ctx);
      mu_layout_row(ctx, 1, (int[]) { -1 }, 0);
      mu_text(ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
        "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
        "ipsum, eu varius magna felis a nulla.");
      mu_layout_end_column(ctx);
    }

    /* background color sliders */
    if (mu_header_ex(ctx, "Background Color", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 74);
      /* sliders */
      mu_layout_begin_column(ctx);
      mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
      mu_label(ctx, "Red:");   mu_slider(ctx, &bg[0], 0, 255);
      mu_label(ctx, "Green:"); mu_slider(ctx, &bg[1], 0, 255);
      mu_label(ctx, "Blue:");  mu_slider(ctx, &bg[2], 0, 255);
      mu_layout_end_column(ctx);
      /* color preview */
      mu_Rect r = mu_layout_next(ctx);
      mu_draw_rect(ctx, r, mu_color(bg[0], bg[1], bg[2], 255));
      char buf[32];
      sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
      mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
    }

    mu_end_window(ctx);
  }
}


static void log_window(mu_Context *ctx) {
  if (mu_begin_window(ctx, "Log Window", mu_rect(350, 40, 300, 200))) {
    /* output text panel */
    mu_layout_row(ctx, 1, (int[]) { -1 }, -25);
    mu_begin_panel(ctx, "Log Output");
    mu_Container *panel = mu_get_current_container(ctx);
    mu_layout_row(ctx, 1, (int[]) { -1 }, -1);
    mu_text(ctx, logbuf);
    mu_end_panel(ctx);
    if (logbuf_updated) {
      panel->scroll.y = panel->content_size.y;
      logbuf_updated = 0;
    }

    /* input textbox + submit button */
    static char buf[128];
    int submitted = 0;
    mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (mu_textbox(ctx, buf, sizeof(buf)) & MU_RES_SUBMIT) {
      mu_set_focus(ctx, ctx->last_id);
      submitted = 1;
    }
    if (mu_button(ctx, "Submit")) { submitted = 1; }
    if (submitted) {
      write_log(buf);
      buf[0] = '\0';
    }

    mu_end_window(ctx);
  }
}


static int uint8_slider(mu_Context *ctx, unsigned char *value, int low, int high) {
  static float tmp;
  mu_push_id(ctx, &value, sizeof(value));
  tmp = *value;
  int res = mu_slider_ex(ctx, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);
  *value = tmp;
  mu_pop_id(ctx);
  return res;
}


static void style_window(mu_Context *ctx) {
  static struct { const char *label; int idx; } colors[] = {
    { "text:",         MU_COLOR_TEXT        },
    { "border:",       MU_COLOR_BORDER      },
    { "windowbg:",     MU_COLOR_WINDOWBG    },
    { "titlebg:",      MU_COLOR_TITLEBG     },
    { "titletext:",    MU_COLOR_TITLETEXT   },
    { "panelbg:",      MU_COLOR_PANELBG     },
    { "button:",       MU_COLOR_BUTTON      },
    { "buttonhover:",  MU_COLOR_BUTTONHOVER },
    { "buttonfocus:",  MU_COLOR_BUTTONFOCUS },
    { "base:",         MU_COLOR_BASE        },
    { "basehover:",    MU_COLOR_BASEHOVER   },
    { "basefocus:",    MU_COLOR_BASEFOCUS   },
    { "scrollbase:",   MU_COLOR_SCROLLBASE  },
    { "scrollthumb:",  MU_COLOR_SCROLLTHUMB },
    { NULL }
  };

  if (mu_begin_window(ctx, "Style Editor", mu_rect(350, 250, 300, 240))) {
    int sw = mu_get_current_container(ctx)->body.w * 0.14;
    mu_layout_row(ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
    for (int i = 0; colors[i].label; i++) {
      mu_label(ctx, colors[i].label);
      uint8_slider(ctx, &ctx->style->colors[i].r, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].g, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].b, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].a, 0, 255);
      mu_draw_rect(ctx, mu_layout_next(ctx), ctx->style->colors[i]);
    }
    mu_end_window(ctx);
  }
}


static void process_frame(mu_Context *ctx) {
  mu_begin(ctx);
  style_window(ctx);
  log_window(ctx);
  test_window(ctx);
  mu_end(ctx);
}



static int text_width(mu_Font font, const char *text, int len) {
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
  return r_get_text_height();
}


int main(int argc, char **argv) {
  /* init renderer */
  r_init();

  /* init microui */
  mu_Context *ctx = malloc(sizeof(mu_Context));
  mu_init(ctx);
  ctx->text_width = text_width;
  ctx->text_height = text_height;

  /* main loop */
  for (;;) {
    /* handle SDL events */
    r_Event e;
    while (r_PollEvent(&e)) {
      switch (e.type) {
        case R_EV_QUIT: exit(EXIT_SUCCESS); break;
        case R_EV_MOUSEMOTION: mu_input_mousemove(ctx, e.motion.x, e.motion.y); break;
        case R_EV_MOUSEWHEEL: mu_input_scroll(ctx, 0, e.wheel.y * -30); break;
        case R_EV_TEXTINPUT: mu_input_text(ctx, e.text.text); break;

        case R_EV_MOUSEBUTTONDOWN:
        case R_EV_MOUSEBUTTONUP: {
          int b = r_button_map[e.button.button & 0xff];
          if (b && e.type == R_EV_MOUSEBUTTONDOWN) { mu_input_mousedown(ctx, e.button.x, e.button.y, b); }
          if (b && e.type ==   R_EV_MOUSEBUTTONUP) { mu_input_mouseup(ctx, e.button.x, e.button.y, b);   }
          break;
        }

        case R_EV_KEYDOWN:
        case R_EV_KEYUP: {
          int c = r_get_event_key_modifier(e);
          if (c && e.type == R_EV_KEYDOWN) { mu_input_keydown(ctx, c); }
          if (c && e.type ==   R_EV_KEYUP) { mu_input_keyup(ctx, c);   }
          break;
        }
      }
    }

    /* process frame */
    process_frame(ctx);

    /* render */
    r_clear(mu_color(bg[0], bg[1], bg[2], 255));
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
        case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
        case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
        case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
        case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
      }
    }
    r_present();
  }

  return 0;
}


