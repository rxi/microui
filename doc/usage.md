# Usage
* **[Overview](#overview)**
* **[Getting Started](#getting-started)**
* **[Layout System](#layout-system)**
* **[Style Customisation](#style-customisation)**
* **[Custom Controls](#custom-controls)**

## Overview
The overall structure when using the library is as follows:
```
initialise `mu_Context`

main loop:
  call `mu_input_...` functions
  call `mu_begin()`
  process ui
  call `mu_end()`
  iterate commands using `mu_command_next()`
```

## Getting Started
Before use a `mu_Context` should be initialised:
```c
mu_Context *ctx = malloc(sizeof(mu_Context));
mu_init(ctx);
```

Following which the context's `text_width` and `text_height` callback functions
should be set:
```c
ctx->text_width = text_width;
ctx->text_height = text_height;
```

In your main loop you should first pass user input to microui using the
`mu_input_...` functions. It is safe to call the input functions multiple times
if the same input event occurs in a single frame.

After handling the input the `mu_begin()` function must be called before
processing your UI:
```c
mu_begin(ctx);
```

Before any controls can be used we must begin a window using one of the
`mu_begin_window...` or `mu_begin_popup...` functions. The `mu_begin_...` window
functions return a truthy value if the window is open, if this is not the case
we should not process the window any further. When we are finished processing
the window's ui the `mu_end_...` window function should be called.

```c
if (mu_begin_window(ctx, "My Window", mu_rect(10, 10, 300, 400))) {
  /* process ui here... */
  mu_end_window(ctx);
}
```

It is safe to nest `mu_begin_window()` calls, this can be useful for things like
context menus; the windows will still render separate from one another like
normal.

While inside a window block we can safely process controls. Controls that allow
user interaction return a bitset of `MU_RES_...` values. Some controls — such
as buttons — can only potentially return a single `MU_RES_...`, thus their
return value can be treated as a boolean:
```c
if (mu_button(ctx, "My Button")) {
  printf("'My Button' was pressed\n");
}
```

The library generates unique IDs for controls internally to keep track of which
are focused, hovered, etc. These are typically generated from the name/label
passed to the function, or, in the case of sliders and checkboxes the value
pointer. An issue arises then if you have several buttons in a window or panel
that use the same label. The `mu_push_id()` and `mu_pop_id()` functions are
provided for such situations, allowing you to push additional data that will be
mixed into the unique ID:
```c
for (int i = 0; i < 10; i++) {
  mu_push_id(ctx, &i, sizeof(i));
  if (mu_button(ctx, "x")) {
    printf("Pressed button %d\n", i);
  }
  mu_pop_id(ctx);
}
```

When we're finished processing the UI for this frame the `mu_end()` function
should be called:
```c
mu_end(ctx);
```

When we're ready to draw the UI the `mu_next_command()` can be used to iterate
the resultant commands. The function expects a `mu_Command` pointer initialised
to `NULL`. It is safe to iterate through the commands list any number of times:
```c
mu_Command *cmd = NULL;
while (mu_next_command(ctx, &cmd)) {
  if (cmd->type == MU_COMMAND_TEXT) {
    render_text(cmd->text.font, cmd->text.text, cmd->text.pos.x, cmd->text.pos.y, cmd->text.color);
  }
  if (cmd->type == MU_COMMAND_RECT) {
    render_rect(cmd->rect.rect, cmd->rect.color);
  }
  if (cmd->type == MU_COMMAND_ICON) {
    render_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
  }
  if (cmd->type == MU_COMMAND_CLIP) {
    set_clip_rect(cmd->clip.rect);
  }
}
```

See the [`demo`](../demo) directory for a usage example.


## Layout System
The layout system is primarily based around *rows* — Each row
can contain a number of *items* or *columns* each column can itself
contain a number of rows and so forth. A row is initialised using the
`mu_layout_row()` function, the user should specify the number of items
on the row, an array containing the width of each item, and the height
of the row:
```c
/* initialise a row of 3 items: the first item with a width
** of 90 and the remaining two with the width of 100 */
mu_layout_row(ctx, 3, (int[]) { 90, 100, 100 }, 0);
```
When a row is filled the next row is started, for example, in the above
code 6 buttons immediately after would result in two rows. The function
can be called again to begin a new row.

As well as absolute values, width and height can be specified as `0`
which will result in the Context's `style.size` value being used, or a
negative value which will size the item relative to the right/bottom edge,
thus if we wanted a row with a small button at the left, a textbox filling
most the row and a larger button at the right, we could do the following:
```c
mu_layout_row(ctx, 3, (int[]) { 30, -90, -1 }, 0);
mu_button(ctx, "X");
mu_textbox(ctx, buf, sizeof(buf));
mu_button(ctx, "Submit");
```

If the `items` parameter is `0`, the `widths` parameter is ignored
and controls will continue to be added to the row at the width last
specified by `mu_layout_width()` or `style.size.x` if this function has
not been called:
```c
mu_layout_row(ctx, 0, NULL, 0);
mu_layout_width(ctx, -90);
mu_textbox(ctx, buf, sizeof(buf));
mu_layout_width(ctx, -1);
mu_button(ctx, "Submit");
```

A column can be started at any point on a row using the
`mu_layout_begin_column()` function. Once begun, rows will act inside
the body of the column — all negative size values will be relative to
the column's body as opposed to the body of the container. All new rows
will be contained within this column until the `mu_layout_end_column()`
function is called.

Internally controls use the `mu_layout_next()` function to retrieve the
next screen-positioned-Rect and advance the layout system, you should use
this function when making custom controls or if you want to advance the
layout system without placing a control.

The `mu_layout_set_next()` function is provided to set the next layout
Rect explicitly. This will be returned by `mu_layout_next()` when it is
next called. By using the `relative` boolean you can choose to provide
a screen-space Rect or a Rect which will have the container's position
and scroll offset applied to it. You can peek the next Rect from the
layout system by using the `mu_layout_next()` function to retrieve it,
followed by `mu_layout_set_next()` to return it:
```c
mu_Rect rect = mu_layout_next(ctx);
mu_layout_set_next(ctx, rect, 0);
```

If you want to position controls arbitrarily inside a container the
`relative` argument of `mu_layout_set_next()` should be true:
```c
/* place a (40, 40) sized button at (300, 300) inside the container: */
mu_layout_set_next(ctx, mu_rect(300, 300, 40, 40), 1);
mu_button(ctx, "X");
```
A Rect set with `relative` true will also effect the `content_size`
of the container, causing it to effect the scrollbars if it exceeds the
width or height of the container's body.


## Style Customisation
The library provides styling support via the `mu_Style` struct and, if you
want greater control over the look, the `draw_frame()` callback function.

The `mu_Style` struct contains spacing and sizing information, as well
as a `colors` array which maps `colorid` to `mu_Color`. The library uses
the `style` pointer field of the context to resolve colors and spacing,
it is safe to change this pointer or modify any fields of the resultant
struct at any point. See [`microui.h`](../src/microui.h) for the struct's
implementation.

In addition to the style struct the context stores a `draw_frame()`
callback function which is used whenever the *frame* of a control needs
to be drawn, by default this function draws a rectangle using the color
of the `colorid` argument, with a one-pixel border around it using the
`MU_COLOR_BORDER` color.


## Custom Controls
The library exposes the functions used by built-in controls to allow the
user to make custom controls. A control should take a `mu_Context*` value
as its first argument and return a `MU_RES_...` value. Your control's
implementation should use `mu_layout_next()` to get its destination
Rect and advance the layout system. `mu_get_id()` should be used with
some data unique to the control to generate an ID for that control and
`mu_update_control()` should be used to update the context's `hover`
and `focus` values based on the mouse input state.

The `MU_OPT_HOLDFOCUS` opt value can be passed to `mu_update_control()`
if we want the control to retain focus when the mouse button is released
— this behaviour is used by textboxes which we want to stay focused
to allow for text input.

A control that acts as a button which displays an integer and, when
clicked increments that integer, could be implemented as such:
```c
int incrementer(mu_Context *ctx, int *value) {
  mu_Id     id = mu_get_id(ctx, &value, sizeof(value));
  mu_Rect rect = mu_layout_next(ctx);
  mu_update_control(ctx, id, rect, 0);

  /* handle input */
  int res = 0;
  if (ctx->mouse_pressed == MU_MOUSE_LEFT && ctx->focus == id) {
    (*value)++;
    res |= MU_RES_CHANGE;
  }

  /* draw */
  char buf[32];
  sprintf(buf, "%d", *value);
  mu_draw_control_frame(ctx, id, rect, MU_COLOR_BUTTON, 0);
  mu_draw_control_text(ctx, buf, rect, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);

  return res;
}
```
