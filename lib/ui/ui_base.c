#include "recov_gui/ui_base.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_ttf.h"
#include "recov_gui/ui_button.h"
#include "recov_gui/ui_container.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_rect.h"
#include <stdio.h>
#include <stdlib.h>

static TTF_Font *global_font = NULL;

void
UIBaseUpdate(UIBase *base, SDL_Renderer *renderer)
{
    UIColor bg = base->background;
    UIRect rect = base->rect;
    SDL_FRect frect = UIRectToFRect(rect);

    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &frect);

    if (base->is_a_container)
    {
        UIContainerUpdateChildren((UIContainer *)base, renderer);
    }
    SDL_RenderPresent(renderer);
}

void
UIBaseOnMouseEnterdefault(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;
    (void)renderer;    
}

void
UIBaseOnFocusdefault(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;
    (void)renderer;    
}

void
UIBaseOnMouseExitdefault(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;
    (void)renderer;
}

void
UIBaseOnMouseDowndefault(UIBase *base, SDL_MouseButtonEvent *event)
{
    (void)base;
    (void)event;
}

void
UIBaseOnMouseUpdefault(UIBase *base, SDL_MouseButtonEvent *event)
{
    (void)base;
    (void)event;
}

void
UIBaseOnMouseClickdefault(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;    
    (void)renderer;
}

bool
UISetGlobalFont(const char *font_name, int font_size)
{
    global_font = TTF_OpenFont(font_name, font_size);
    if (!global_font)
        return false;
    return true;
}

TTF_Font *
UIGetGlobalFont()
{
    return global_font;
}

UIColor
UIColorInit(int r, int g, int b, int a)
{
    return (UIColor){r, g, b, a};
}

UIBase *
UIBaseInit(size_t size, UIRect rect, UIComponentType type, UIOnFocusFn on_focus,
           UIUpdateFn on_update, void *user_data)
{
    UIBase *base = malloc(size < sizeof(UIBase) ? sizeof(UIBase) : size);
    base->rect = rect;
    base->is_focused = false;
    base->on_focus = on_focus ? on_focus : UIBaseOnFocusdefault;
    base->on_update = on_update ? on_update : UIBaseUpdate;
    base->on_mouse_enter = UIBaseOnMouseEnterdefault;
    base->on_mouse_exit = UIBaseOnMouseExitdefault;
    base->on_mouse_down = UIBaseOnMouseDowndefault;
    base->on_mouse_up = UIBaseOnMouseUpdefault;
    base->on_mouse_click = UIBaseOnMouseClickdefault;
    base->user_data = user_data;
    base->type = type;
    base->foreground = UIColorInit(255, 255, 255, 255);
    base->background = UIColorInit(0, 0, 0, 255);
    UIBaseSetLayoutType(base, UI_LAYOUT_ABSOLUTE);
    return base;
}

bool
UIBaseIsFocused(UIBase *base)
{
    return base ? base->is_focused : false;
}

void
UISetFocusFn(UIBase *base, UIOnFocusFn fn)
{
    base->on_focus = fn;
}

void
UISetUpdateFn(UIBase *base, UIUpdateFn fn)
{
    base->on_update = fn;
}

void
UIBaseSetBackGround(UIBase *base, UIColor color)
{   
    base->background = color;
}

void
UIBaseSetForeGround(UIBase *base, UIColor color)
{
    base->foreground = color;
}

void
UIBaseSetLayoutType(UIBase *base, UILayoutType type)
{
    base->layout = type;
}

void
UIBaseSetOnClickCB(UIBase *base, UIOnMouseClickFn fn)
{
    base->on_mouse_click = fn;
}

void
UIBaseSetOnMouseEnterCB(UIBase *base, UIOnMouseEnterFn fn)
{
    
    base->on_mouse_enter = fn;
}

void
UIBaseSetOnMouseExitCB(UIBase *base, UIOnMouseExitFn fn)
{
    base->on_mouse_exit = fn;
}
