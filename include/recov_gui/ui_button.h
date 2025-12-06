#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "recov_gui/ui_base.h"
#include <stdint.h>

typedef struct UIButton UIButton;
typedef int (*UIButtonCB) (UIButton *);

typedef struct UIButton
{
    UIBase base;
    UIColor hover;
    UIColor background;
    const char *label;
    SDL_Surface *surface;
    SDL_Texture *texture;
    int text_w, text_h;
    bool hover_set;    
} UIButton;

UIButton *UIButtonInit(int64_t width, int64_t height, int64_t xpos,
                       int64_t ypos, const char *label);

void UIButtonOnClick(UIButton *button, UIOnMouseClickFn fn);
