#include "recov_gui/ui_label.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_ttf.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_rect.h"

static void
UILabelUpdateDefault(UIBase *base, SDL_Renderer *renderer)
{
    UILabel *label = (UILabel*)base;
    UIRect rect = base->rect;
    UIColor bg = base->background;
    SDL_FRect frect = UIRectToFRect(rect);

    // SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    // SDL_RenderFillRect(renderer, &frect);

    if (label->texture == NULL)
    {
        label->texture = SDL_CreateTextureFromSurface(renderer, label->surface);
        label->text_h = label->texture->h;        
	label->text_w = label->texture->w;
        SDL_DestroySurface(label->surface);
    }

    SDL_FRect text_rect;
    text_rect.x = rect.x + (rect.w - label->text_w) / 2.0f;
    text_rect.y = rect.y + (rect.h - label->text_h) / 2.0f;
    text_rect.w = label->text_w;
    text_rect.h = label->text_h;

    SDL_RenderTexture(renderer, label->texture, NULL, &text_rect);
}    

UILabel *
UILabelInit(int64_t width, int64_t height, int64_t xpos, int64_t ypos,
            const char *text)
{
    return UILabelInitRect(UIRectInit(width, height, xpos, ypos), text);
}

UILabel *
UILabelInitRect(UIRect rect, const char *text)
{
    UILabel *label = (UILabel *)UIBaseInit(sizeof(UILabel), rect, UICOMP_Label,
                                           NULL, UILabelUpdateDefault, NULL);
    label->text = text;
    label->surface = TTF_RenderText_Blended(
        UIGetGlobalFont(), text, strlen(text), label->base.foreground);
    label->texture = NULL;
    return label;
}
