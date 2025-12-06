#include "recov_gui/ui_container.h"
#include "SDL3/SDL_render.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_rect.h"
#include "shared/array.h"
#include <stdio.h>

void
UIContainerUpdateChildren(UIContainer *container, SDL_Renderer *renderer)
{
    arrForEach(&container->children, child)
    {
        UIBase *chb = (UIBase *)*child;

        if (chb->on_update)
            chb->on_update(chb, renderer);
    }
}

UIContainer *
UIContainerInit(size_t size, int64_t width, int64_t height, int64_t xpos,
                int64_t ypos, UIComponentType type, UIOnFocusFn on_focus,
                UIUpdateFn on_update, void *user_data)
{
    return UIContainerInitRect(size, UIRectInit(width, height, xpos, ypos),
                               type, on_focus, on_update, user_data);
}

UIContainer *
UIContainerInitRect(size_t size, UIRect rect, UIComponentType type,
                    UIOnFocusFn on_focus, UIUpdateFn on_update, void *user_data)
{
    UIContainer *cont = (UIContainer *)UIBaseInit(size, rect, type, on_focus,
                                                  on_update, user_data);
    arrInit(&cont->children);
    cont->base.is_a_container = true;
    return cont;
}

void
UIContainerAddChild(UIContainer *container, UIBase *child)
{
    UIBase *cbase = UIBASE(container);
    arrAppend(&container->children, child);
    switch (cbase->layout)
    {
    case UI_LAYOUT_RELATIVE: {
        // A Window's position is relative to the screen so we
        // can't add those values to the component.
        if (cbase->type != UICOMP_Window)
        {
            child->rect.x += cbase->rect.x;
            child->rect.y += cbase->rect.y;
        }
    }
    break;
    default:
        break;
    }
}
