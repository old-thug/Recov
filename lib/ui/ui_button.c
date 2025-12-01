#include "recov_gui/ui_button.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_ttf.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_rect.h"

static void
UIButtonOnFocusDefault (UIBase *base)
{
}

static void
UIButtonOnUpdateDefault (UIBase *base, SDL_Renderer *renderer)    
{
    UIButton *button = (UIButton *)base;
    UIRect rect = base->rect;
    UIColor bg = base->background;
    SDL_FRect frect = UIRectToFRect(rect);

    SDL_SetRenderDrawColor (renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect (renderer, &frect);
    
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // light border
    SDL_RenderRect (renderer, &frect);

    if (button->texture == NULL)        
    {
        button->texture
            = SDL_CreateTextureFromSurface (renderer, button->surface);       
        button->text_h = button->texture->h;
        button->text_w = button->texture->w;        
        SDL_DestroySurface (button->surface);        
    }

    SDL_FRect text_rect;
    text_rect.x = rect.x + (rect.w - button->text_w) / 2.0f;
    text_rect.y = rect.y + (rect.h - button->text_h) / 2.0f;
    text_rect.w = button->text_w;
    text_rect.h = button->text_h;    
    
    SDL_RenderTexture (renderer, button->texture, NULL, &text_rect);
}    

UIButton *
UIButtonInit (int64_t width, int64_t height, int64_t xpos, int64_t ypos,
              const char *label)
{
    UIButton *button = (UIButton *)UIBaseInit (
        sizeof (UIButton), UIRectInit (width, height, xpos, ypos),
        UICOMP_Button, UIButtonOnFocusDefault, UIButtonOnUpdateDefault, NULL);
    button->label = label;
    UIBase *base = (UIBase *)button;
    TTF_Font *font = UIGetGlobalFont ();
    button->surface
        = TTF_RenderText_Blended (font, label, strlen(label), base->foreground);
    if (!button->surface)
        return NULL;    
    button->texture = NULL;
    return button;    
}

void
UIButtonOnClick(UIButton *button, UIOnMouseClickFn fn)
{
    UIBaseSetOnClickCB(UIBASE(button), fn);    
}    
