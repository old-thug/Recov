#include "recov_gui/ui_rect.h"
#include "SDL3/SDL_rect.h"


UIRect
UIRectInit (int64_t width, int64_t height, int64_t xpos, int64_t ypos)
{
    UIRect rect;
    rect.w = width;
    rect.h = height;
    rect.x = xpos;
    rect.y = ypos;    
    return rect;    
}


SDL_FRect
UIRectToFRect (UIRect rect)
{
    SDL_FRect frect;
    frect.w = rect.w;
    frect.h = rect.h;
    frect.x = rect.x;
    frect.y = rect.y;    
    return frect;
}    
