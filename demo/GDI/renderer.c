#define WIN32_LEAN_AND_MEAN
#define INT_MAX (int) ((unsigned) -1 / 2)

#include "renderer.h"
#include <stdio.h>
#include "microui.h"

static int width = 800;
static int height = 600;

int running;
static int needs_refresh = 1;

static HWND window;
static HDC dc;

static RECT Client_Rect;
static int win_width;
static int win_height;
static PAINTSTRUCT ps;
static HDC Memhdc;

static HBITMAP Membitmap;

HFONT hFont;
//static TEXTMETRICW lptm;
static int drag = 0;


int size_t2int(size_t val) {
    return (val <= INT_MAX) ? (int)((size_t)val) : -1;
}

char* numToASCII(int num) {
    char* string = malloc(2);
    if (!string)
        return 0;
    string[0] = num;
    string[1] = 0;
    return string;
}

/*unused we have our own window proc*/
/*this might need to be attached differently ? */
static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

    if(msg ==  WM_DESTROY)
    {
        running=0;
        PostQuitMessage(0);
        return 0;
    }
   
    return DefWindowProc(wnd, msg, wparam, lparam);
}

void r_init(void) {
    running = 1;
    /* Win32 Window */
    WNDCLASSA wc;

    RECT rect = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exstyle = WS_EX_APPWINDOW;
    HWND wnd;

    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "microui-gdi";
    ATOM atom = RegisterClassA(&wc);
    if (!atom) {
        printf("error registering window...\n");

    }
    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    wnd = CreateWindowExA(exstyle, wc.lpszClassName, "microui-gdi-demo",
        style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc.hInstance, NULL);
    window = wnd;
    hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
    //GetTextMetricsA(hFont, &lptm);

}

static void push_quad(mu_Rect dst, mu_Color color) {
    if (color.a < 1) {
        /*no real alpha!*/
        return;
    }

    RECT rect;
    SetRect(&rect, dst.x, dst.y, dst.x + dst.w, dst.y + dst.h);
    
    SetBkColor(Memhdc, RGB(color.r, color.g, color.b));
    ExtTextOutA(Memhdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

}

void r_draw_rect(mu_Rect rect, mu_Color color) {

    push_quad(rect, color);
}

void r_draw_text(const char* text, mu_Vec2 pos, mu_Color color) {

    SelectObject(Memhdc, hFont);
    SetBkMode(Memhdc, TRANSPARENT);
    SetTextColor(Memhdc, RGB(color.r, color.g, color.b));
    ExtTextOutA(Memhdc, pos.x, pos.y, ETO_OPAQUE, NULL, (LPCSTR)text, strlen(text), NULL);

}

void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
    
    int c=0, w=0, h=0;
    mu_Vec2 pos;
    char buf[2];
    switch (id) {
    case MU_ICON_CLOSE:		c = 'x'; break;
    case MU_ICON_CHECK:		c = 'X'; break;
    case MU_ICON_COLLAPSED:	c = '>'; break;
    case MU_ICON_EXPANDED:	c = 'v'; break;
    //case MU_ICON_RESIZE:	c = '+'; break;
    }
    buf[0] = c;
    buf[1] = 0;
    w = r_get_text_width(buf, 1);
    h = r_get_text_height();
    pos.x = rect.x + (rect.w - w) / 2;
    pos.y = rect.y + (rect.h - h) / 2;
    r_draw_text(buf, pos, color);
}

int r_get_text_width(const char* text, int len) {
    int chrlen = 0;
    int strlen = 0;
    for (int i = 0; i < len; i++) {
        chrlen = 0;
        GetCharWidthA(Memhdc, text[i], text[i],&chrlen);
        strlen += chrlen;

    }
    return strlen;
}



int _r_get_text_width(const char* text, int len)
{
    return 32;
    SIZE size;
    SelectObject(Memhdc, hFont);
    GetTextExtentPoint32A(Memhdc, text, size_t2int(strlen(text)), &size);
    return size_t2int(size.cx);
}
int r_get_text_height(void) {
    SIZE size;
    SelectObject(Memhdc, hFont);
    GetTextExtentPoint32A(Memhdc, "E", 1, &size);
    return size_t2int(size.cy);
}

void r_set_clip_rect(mu_Rect rect) {

    SelectClipRgn(Memhdc, NULL);
    IntersectClipRect(Memhdc, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
}


void r_clear(mu_Color color) {
    
    RECT rect;
    SetRect(&rect, Client_Rect.left, Client_Rect.top, Client_Rect.right, Client_Rect.bottom);
    SetBkColor(Memhdc, RGB(color.r, color.g, color.b));
    //this lets us draw with no bruhes... I guess?
    ExtTextOutA(Memhdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

void r_begin(void) {

    DeleteDC(Memhdc);

    /* bootleg flip (buffer swap) clean last frame */
    /* we delete the oppositie buffer of render end */
    /* I need to confirm I dont have any stray undelted objects leaking... */

    /* Begin drawing */

    GetClientRect(window, &Client_Rect);
    InvalidateRect(window, &Client_Rect, FALSE);
    win_width = Client_Rect.right - Client_Rect.left;
    win_height = Client_Rect.bottom + Client_Rect.left;
    dc = BeginPaint(window, &ps);

    Memhdc = CreateCompatibleDC(dc);
    Membitmap = CreateCompatibleBitmap(dc, win_width, win_height);
    SelectObject(Memhdc, Membitmap);

}

void r_end(void) {
    /* End drawling */
    BitBlt(dc, 0, 0, win_width, win_height, Memhdc, 0, 0, SRCCOPY);

    /* bootleg flip (buffer swap) */
    DeleteDC(dc);
    DeleteObject(Membitmap);
    EndPaint(window, &ps);
    /*not sure if this is needed?*/
    DeleteObject((HGDIOBJ)ps.hdc);
}



void r_handle_input(mu_Context* ctx)
{
    /* Input messages */
    MSG msg;

    if (needs_refresh == 0) {
        if (GetMessageW(&msg, NULL, 0, 0) <= 0)
            running = 0;
        else {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        needs_refresh = 1;
    }
    else needs_refresh = 0;


    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
        {
            running = 0;
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
        r_handle_input(ctx);
        needs_refresh = 1;

    }


    switch (msg.message) {

    case WM_CREATE: {
        return;
    }
    case WM_SIZE:
    {
        return;
    }
    case WM_DRAWITEM:
    {
        return;
    }
    case WM_ERASEBKGND:
    {
        return;
    }
    case WM_PAINT:
    {
        /*skip this we do it ourself*/
        return;
    }

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
        int down = !((msg.lParam >> 31) & 1);
        int ctrl = GetKeyState(VK_CONTROL) & (1 << 15);
        switch (msg.wParam)
        {
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
                mu_input_keydown(ctx, MU_KEY_SHIFT);

            return;
        case VK_BACK:
        case VK_DELETE:
             mu_input_keydown(ctx, MU_KEY_BACKSPACE);
            return;
        case VK_RETURN:     
            mu_input_keydown(ctx, MU_KEY_RETURN);            
            return;
        case VK_MENU:
        case VK_RMENU:
        case VK_LMENU:
            if (down) {
                mu_input_keydown(ctx, MU_KEY_ALT);
            }
            return;
        case VK_TAB:
        case VK_LEFT:
        case VK_RIGHT:
        case VK_HOME:
        case VK_END:
        case VK_NEXT:
        case VK_PRIOR:
            /*not handled*/
            return;
            return;
        }
    }

    case WM_CHAR: {
        if (msg.wParam >= 32)
        {
            int down = !((msg.lParam >> 31) & 1);
            if (down) {
                char* result = numToASCII(msg.wParam);
                mu_input_text(ctx, result);
                free(result);
            }
        }
    }
    case WM_MOUSEMOVE: {
        mu_input_mousemove(ctx, GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));

        //this logic could be simplified but it works
        if (msg.wParam != MK_LBUTTON) {
            drag = 0;
        }
        if (drag) {
            return;
        }
    }

    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    {

        if (msg.wParam != MK_RBUTTON) {
            mu_input_mouseup(ctx, GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam), 2);
            ReleaseCapture();
        }
        if (msg.wParam != MK_LBUTTON)
        {
            mu_input_mouseup(ctx, GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam), 1);
            ReleaseCapture();
        }
        if (msg.wParam == MK_RBUTTON) {
            mu_input_mousedown(ctx, GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam), 2);
            SetCapture(window);           
            drag = 1;
        }
        if (msg.wParam == MK_LBUTTON) {
            mu_input_mousedown(ctx, GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam), 1);
            SetCapture(window);
            drag = 1;
        }
    }
    default: {
        return;
    }
    }
}