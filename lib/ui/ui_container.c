#include "recov_gui/ui_container.h"
#include "SDL3/SDL_render.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_rect.h"
#include "shared/array.h"
#include <stdio.h>


void
UIContainerUpdateChildren (UIContainer *container, SDL_Renderer *renderer)
{
    UIBase *cbase = (UIBase *)container;
    arrForEach (&container->children, child)
    {
        UIBase *chb = (UIBase *)*child;

        switch (chb->layout.type)
            {                
            case UI_LAYOUT_ABSOLUTE:
                {
                    chb->rect = UIRectInit (
                        chb->layout.absolute.w, chb->layout.absolute.h,
                        chb->layout.absolute.x, chb->layout.absolute.y);
                }                
                break;
            case UI_LAYOUT_RELATIVE:
                {
                    // A Window's position is relative to the screen so we
                    // can't add those values to the component.                    
                    if (cbase->type != UICOMP_Window)
                        {
                            chb->rect.x = cbase->rect.x + chb->rect.x;
                            chb->rect.y = cbase->rect.y + chb->rect.y;                            
			}                
                }
                break;
            default:
                break;
            }
        
        
	if (chb->on_update)
	    chb->on_update (chb, renderer);        
    }        
}    

UIContainer *
UIContainerInitRect (size_t size, UIRect rect, UIComponentType type, UIOnFocusFn on_focus, UIUpdateFn on_update, void *user_data)
{
    UIContainer *cont = (UIContainer *)UIBaseInit (size, rect, type, on_focus,
                                                   on_update, user_data);
    arrInit(&cont->children);
    cont->base.is_a_container = true;
    return cont;
}

void
UIContainerAddChild (UIContainer *container, UIBase *child)
{
    arrAppend (&container->children, child);    
}    
