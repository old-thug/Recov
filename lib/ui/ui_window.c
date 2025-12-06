#include "recov_gui/ui_window.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_container.h"
#include "recov_gui/ui_rect.h"
#include "shared/array.h"

static void
UIRenderWindowDefault(UIBase *base, SDL_Renderer *renderer)
{
    UIWindow *window = (UIWindow *)base;
    (void)renderer;

    UIColor win_color = base->background;
    SDL_SetRenderDrawColor(window->renderer, win_color.r, win_color.g, win_color.b, win_color.a);
    SDL_RenderClear(window->renderer);

    UIContainer *container = (UIContainer *)window;
    UIContainerUpdateChildren(container, renderer);
    SDL_RenderPresent(window->renderer);
}

static void
UIWindowFocusDefault(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;
    (void)renderer;    
    // STUB.
}

UIWindow *
UIWindowInit(int64_t width, int64_t height, int64_t xpos, int64_t ypos,
             const char *title, void *user_data)
{
    UIWindow *window = (UIWindow *)UIContainerInitRect(
        sizeof(UIWindow), UIRectInit(width, height, xpos, ypos), UICOMP_Window,
        UIWindowFocusDefault, UIRenderWindowDefault, user_data);
    window->window_title = title;
    window->window =
        SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    window->renderer = SDL_CreateRenderer(window->window, NULL);
    return window;
}

UIWindow *
UIWindowInitRect(UIRect rect, const char *title, void *user_data)
{
    return UIWindowInit(rect.w, rect.h, rect.x, rect.y, title, user_data);
}
