# Usage
* **[Overview](#overview)**
* **[Getting Started](#getting-started)**
* **[Layout System](#layout-system)**
* **[Style Customisation](#style-customisation)**
* **[Custom Controls](#custom-controls)**

## Overview
The overall structure when using the library is as follows:
```
initialize `mu_Context`

main loop:
  call `mu_input_...` functions
  call `mu_begin()`
  process ui
  call `mu_end()`
  iterate commands using `mu_command_next()`
```

## Getting Started
Before use a `mu_Context` should be initialized:
```c
mu_Context *ctx = malloc(sizeof(mu_Context));
mu_init(ctx);
```

In your main loop you should first pass user input to microui using the
`mu_input_...` functions. It is safe to call the input functions multiple
times if the same input event occurs in a single frame.

After handling the input the `mu_begin()` function must be called before
processing your UI:
```c
mu_begin(ctx);
```

Before any controls can be used we must begin a window using one of the
`mu_begin_window...` or `mu_begin_popup...` functions. The `mu_Container`
for the window is expected to be either zeroed memory in which case
it will be initialized automatically when used, or to have been
intialized manually using the `mu_init_window()` function; once used,
the `mu_Container`'s memory must remain valid until `mu_end()` is called
at the end of the frame. The `mu_begin_...` window functions return a
truthy value if the window is open, if this is not the case we should
not process the window any further. When we are finished processing the
window's ui the `mu_end_...` window function should be called.

```c
static mu_Container window;

if (mu_begin_window(ctx, &window, "My Window")) {
  /* process ui here... */
  mu_end_window(ctx);
}
```

It is safe to nest `mu_begin_window()` calls, this can be useful for
things like context menus; the windows will still render separate from
one another like normal.

While inside a window block we can safely process controls. Controls
that allow user interaction return a bitset of `MU_RES_...` values. Some
controls — such as buttons — can only potentially return a single
`MU_RES_...`, thus their return value can be treated as a boolean:
```c
if (mu_button(ctx, "My Button")) {
  printf("'My Button' was pressed\n");
}
```

The library generates unique IDs for controls internally to keep track
of which are focused, hovered, etc. These are generated either from the
pointer passed to the function (eg. for treenodes, checkboxes, textboxes
and sliders), or the string/icon passed to the function (eg. buttons). An
issue arrises then if you have several buttons in a window or panel that
use the same label. The `mu_push_id()` and `mu_pop_id()` functions are
provided for such situations, allowing you to push additional data that
will be mixed into the unique ID:
```c
for (int i = 0; i < 10; i++) {
  mu_push_id(ctx, &i, sizeof(i));
  if (mu_button(ctx, "x")) {
    printf("Pressed button %d\n", i);
  }
  mu_pop_id(ctx);
}
```

When we're finished processing the UI for this frame the `mu_end()`
function should be called:
```c
mu_end(ctx);
```

When we're ready to draw the UI the `mu_next_command()` can be used
to iterate the resultant commands. The function expects a `mu_Command`
pointer initialized to `NULL`. It is safe to iterate through the commands
list any number of times:
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
*TODO*


## Style Customisation
*TODO*


## Custom Controls
*TODO*

