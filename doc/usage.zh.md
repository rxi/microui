# 使用方法
* **[概述](#概述)**
* **[使用入门](#使用入门)**
* **[布局系统](#布局系统)**
* **[风格自定义](#风格自定义)**
* **[自定义控件](#自定义控件)**

## 概述
在程序中使用本库时的总体架构：
```
初始化 `mu_Context`

主循环：
  调用 `mu_input_...` 系列函数
  调用 `mu_begin()`
  设置和处理 UI
  调用 `mu_end()`
  使用 `mu_command_next()` 不断获取指令，执行之
```

## 使用入门
在使用 `mu_Context` 结构体前应对其进行初始化：
```c
mu_Context *ctx = malloc(sizeof(mu_Context));
mu_init(ctx);
```

接下来应当设置 `text_width` 和 `text_height` 这两个回调函数：
```c
ctx->text_width = text_width;
ctx->text_height = text_height;
```

在主循环中你首先应该使用 `mu_input_...` 系列函数将用户输入传给 microui。在同一
帧里多次调用同一输入函数是安全的。

处理完输入后，在设置 UI 界面前必须调用 `mu_begin()`：
```c
mu_begin(ctx);
```

在创建任何 UI 控件之前我们必须先用某个 `mu_begin_window...` 或 
`mu_begin_popup...` 函数开启一个窗口上下文。用于开启窗口上下文的 `mu_begin_...` 
系列函数在窗口开启时会返回一个非 0 值；若返回 0，说明该窗口不会开启，在接下来的
程序里不应该布置该窗口的 UI。当我们布置好一个窗口后应该调用对应的 `mu_end_...` 
函数。
```c
if (mu_begin_window(ctx, "My Window", mu_rect(10, 10, 300, 400))) {
  /* 在此处布置 UI... */
  mu_end_window(ctx);
}
```

嵌套调用 `mu_begin_window()` 是安全的，这个特性在定义上下文菜单这样的使用场景中
很有用；嵌套创造的窗口将像普通窗口一样独立渲染出来。

在一个窗口上下文中我们可以安全地调用 UI 控件函数以布置 UI。允许用户交互的 UI 控件函数会返回一个由 `MU_RES_...` 系列值相与而成的位集值。一些 UI 控制函数——比如
按钮——只会返回一个单独的 `MU_RES_...` 值，因此可以将其返回值视为布尔值。
```c
if (mu_button(ctx, "My Button")) {
  printf("'My Button' was pressed\n");
}
```

本库为每个 UI 控件产生互不相同的 ID 以追踪哪些组件正处于焦点、鼠标悬停在哪个
组件上等等。这些 ID 一般使用传递给对应函数的名字或标签字符串来生成，或者，在
滑动条和复选框的情形下，则是使用传入的指针地址值。如果你在同一个窗口或 panel 
中有一些标签文本相同的按钮，就会导致问题。`mu_push_id()` 和 `mu_pop_id()` 函数
就是为这种情形准备的，它们使您可以向生成 ID 的过程传入额外的数据以生成互不相同
的 ID：
```c
for (int i = 0; i < 10; i++) {
  mu_push_id(ctx, &i, sizeof(i));
  if (mu_button(ctx, "x")) {
    printf("Pressed button %d\n", i);
  }
  mu_pop_id(ctx);
}
```

当我们布置完了一帧的 UI，应该调用 `mu_end()` 函数：
```c
mu_end(ctx);
```

当我们准备好绘制 UI 时就可以使用 `mu_next_command()` 来遍历最终生成的指令了。这
个函数需要一个初始化为 `NULL` 的 `mu_Command` 指针。多次遍历指令列表是安全的：
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

请查看 [`demo`](../demo) 目录里的使用样例。


## 布局系统
microui 的布局系统主要基于*行*——每行可以包含若干*UI控件*或*列*，
每列又可以在其中包含若干行，以此类推。行布局需要用 `mu_layout_row()` 函数
初始化，用户应该指定一行的项数，一个包含每项宽度的数组，以及行高：
```c
/* 初始化了一个包含 3 项的行布局：第一项宽为 90，
** 剩下两项宽都是 100 */
mu_layout_row(ctx, 3, (int[]) { 90, 100, 100 }, 0);
```
一行被填满后下一行自动开始，如果在上述代码后立即布置 6 个按钮，会产生两行。
此函数可被再次调用以开始一个新的行布局。

除了使用正的绝对值长度，行中项的宽度和行高还可以被指定为 `0`，此时会使用
当前 `mu_Context` 中的 `style.size` 值，还可以指定一个负值，则会调整所设
定项的宽度/高度使其与右边/底边的距离为这个负值的绝对值。因此如果我们想要
一个小按钮，一个填充所在行大部分宽度的输入框，一个更大的按钮从左至右排列，
我们可以编写如下代码：
```c
mu_layout_row(ctx, 3, (int[]) { 30, -90, -1 }, 0);
mu_button(ctx, "X");
mu_textbox(ctx, buf, sizeof(buf));
mu_button(ctx, "Submit");
```

若 `items` 参数为 `0`，则 `widths` 参数会被忽略，UI 控件会继续添加到当前
行的最后，其宽度会使用最后一次 `mu_layout_width()` 的调用值（可使用如上所述
的 `0` 和负数），如果没有调用过 `mu_layout_width()` 则使用 `style.size.x`：
```c
mu_layout_row(ctx, 0, NULL, 0);
mu_layout_width(ctx, -90);
mu_textbox(ctx, buf, sizeof(buf));
mu_layout_width(ctx, -1);
mu_button(ctx, "Submit");
```

列布局可出现在一行的任意位置，使用 `mu_layout_begin_column()` 开始。一旦
开始，上述行布局规则将被局限在列布局内——所有以负值指定的相对宽度将相对于
列布局空间的右边界而不是容器的右边界。所有新行将在此列中布局直到调用 
`mu_layout_end_column()`。

UI 控件在内部使用 `mu_layout_next()` 来获取下一个组件以屏幕坐标表示
的位置矩形（`mu_Rect` 类型）并更新布局系统，当你定制自己的 UI 控件时应
该使用这个函数，你也可以用这个函数在不放置新 UI 控件的同时更新布局系统。

`mu_layout_set_next()` 用来显式设置下一个控件的布局位置。传入的 `mu_Rect` 
参数将在下一次调用 `mu_layout_next()` 时返回。布尔值参数 `relative` 可以
控制传入的坐标是屏幕坐标还是基于当前容器的相对坐标。你也可以先调用 
`mu_layout_next()` 获取下一个布局位置再使用 `mu_layout_set_next()` 重新
设定它：
```c
mu_Rect rect = mu_layout_next(ctx);
mu_layout_set_next(ctx, rect, 0);
```

如果你想在当前容器内任意位置放置控件，应用非 0 的真值 `relative` 参数调用 
`mu_layout_set_next()`：
```c
/* place a (40, 40) sized button at (300, 300) inside the container: */
mu_layout_set_next(ctx, mu_rect(300, 300, 40, 40), 1);
mu_button(ctx, "X");
```
随着 `relative` 为真而设置的布局位置也会影响当前容器的 `content_size`，如
果其超出了容器内部区域的长度或宽度，还会影响相应的滚动条状态。


## 风格自定义
本库通过 `mu_Style` 结构体提供风格化支持，如果你想要更好的控件外观，
应该使用 `draw_frame()` 回调函数。

`mu_Style` 结构体包含了控件的间距和尺寸信息，还有 `color` 数组将 `colorid` 
映射为 `mu_Color` 值。本库的 `mu_Context` 上下文结构体使用 `style` 指针成
员来决定部件间的间距和各部分颜色，在任何时候更改这个指针或者修改它指向的 
`style` 结构体都是安全的。您可以在 [`microui.h`](../src/microui.h) 查看相
关结构体的实现。

除了有 `style` 成员指向风格结构体，`mu_Context` 还有一个 `draw_frame()` 
回调函数，每当一个 UI 控件需要绘制自己的矩形背景框时便会调用此回调函数，
其默认行为是用传入的 `colorid` 参数代表的颜色绘制一个矩形，其边框宽度带
为 1 像素，边框颜色使用 `MU_COLOR_BORDER` 代表的颜色。


## 自定义控件
本库对外暴露了内置 UI 控件所使用的函数以允许用户编写自己的 UI 控件。
一个定义 UI 控件的函数应该接受 `mu_Context*` 值作为第一参数，
返回一个 `MU_RES_...` 值。自定义 UI 控件应使用 `mu_layout_next()` 
获取它的布局位置并更新布局系统。应当使用 `mu_get_id()` 和一些特有输入
参数产生部件 ID，并用 `mu_update_control()` 更新上下文基于鼠标输入状态
的 `hover`（悬停）和 `focus`（焦点）值。

若我们想在鼠标按键松开后仍保留控件焦点，应向 `mu_update_control()` 传入 
`MU_OPT_HOLDFOCUS` 选项值——这种行为被应用于文本框，我们希望在输入文字时
一直保持焦点。

下面是一个例子，这个控件表现得像个按钮，它还显示一个整数，每次点击会使
显示的数字加一：
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
