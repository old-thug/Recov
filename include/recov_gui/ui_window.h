#pragma once

#include "recov_gui/ui_container.h"
#include "recov_gui/ui_rect.h"

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"
#include <stdint.h>

typedef struct UIWindow
{
    UIContainer base;
    const char *window_title;
    SDL_Window *window;
    SDL_Renderer *renderer;    
} UIWindow;

UIWindow *UIWindowInit (int64_t width, int64_t height, int64_t xpos,
                        int64_t ypos, const char *title, void *user_data);
UIWindow *UIWindowInitRect (UIRect position, const char *title,
                            void *user_data);
