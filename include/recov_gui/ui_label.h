#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_rect.h"

typedef struct UILabel {
    UIBase base;
    const char *text;
    SDL_Texture *texture;
    SDL_Surface *surface;
    int text_w, text_h;
} UILabel;

UILabel* UILabelInit(int64_t width, int64_t height, int64_t xpos,
		     int64_t ypos, const char *text);
UILabel* UILabelInitRect(UIRect rect, const char *text);
