#pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_ttf.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_rect.h"
#include <stdbool.h>
#include <stddef.h>

#define UIBASE(x) ((UIBase *)(x))

typedef enum
{
    UICOMP_Window,
    UICOMP_Panel,
    UICOMP_Button,
} UIComponentType;

typedef struct UIBase UIBase;
typedef SDL_Color UIColor;
typedef void (*UIOnFocusFn) (UIBase *);
typedef void (*UIUpdateFn)(UIBase *, SDL_Renderer *);
typedef void (*UIOnMouseEnterFn) (UIBase *);
typedef void (*UIOnMouseExitFn) (UIBase *);
typedef void (*UIOnMouseClickFn) (UIBase *);
typedef void (*UIOnMouseDownFn) (UIBase *, SDL_MouseButtonEvent *);
typedef void (*UIOnMouseUpFn) (UIBase *, SDL_MouseButtonEvent *);

typedef struct UIBase
{
    UIRect rect;		/* Final Computed rect */
    UILayout layout;
    UIComponentType type;
    UIOnFocusFn on_focus;
    UIUpdateFn on_update;
    UIOnMouseEnterFn on_mouse_enter;
    UIOnMouseExitFn on_mouse_exit;
    UIOnMouseDownFn on_mouse_down;
    UIOnMouseUpFn on_mouse_up;
    UIOnMouseClickFn on_mouse_click;  
    UIColor foreground;
    UIColor background;    
    bool is_focused;
    void *user_data;
    bool is_a_container;    
} UIBase;

UIBase *UIBaseInit (size_t size, UIRect rect, UIComponentType type, UIOnFocusFn on_focus, UIUpdateFn on_update, void *user_data);
bool UIBaseIsFocused (UIBase *base);
void UIBaseUpdate (UIBase *base);
void UISetFocusFn (UIBase *base, UIOnFocusFn fn);
void UISetUpdateFn (UIBase *base, UIUpdateFn fn);

void UIBaseSetBackGround(UIBase *base, UIColor color);
void UIBaseSetForeGround(UIBase *base, UIColor color);

void UIBaseSetLayout (UIBase *base, UILayout layout);
void UIBaseSetLayoutType (UIBase *base, UILayoutType type);

bool UISetGlobalFont(const char *font_name, int font_size);
TTF_Font *UIGetGlobalFont();

UIColor UIColorInit(int r, int g, int b, int a);

void UIBaseSetOnClickCB(UIBase *base, UIOnMouseClickFn fn);
void UIBaseSetOnMouseEnterCB(UIBase *base, UIOnMouseEnterFn fn);
void UIBaseSetOnMouseExitCB(UIBase *base, UIOnMouseExitFn fn);
