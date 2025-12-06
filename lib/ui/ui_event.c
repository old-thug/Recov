#include "recov_gui/ui_event.h"
#include "SDL3/SDL_events.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_container.h"

static UIBase *
UIFindHit(UIContainer *root, float mx, float my)
{
    for (int64_t i = root->children.count - 1; i >= 0; i--)
    {
        UIBase *child = root->children.buffer[i];
        UIRect r = child->rect;

        if (mx >= r.x && mx < r.x + r.w && my >= r.y && my < r.y + r.h)
        {
            if (child->is_a_container)
            {
                UIBase *deep = UIFindHit((UIContainer *)child, mx, my);
                if (deep)
                    return deep;
            }
            return child;
        }
    }
    return NULL;
}

static void
UIHandleMouse(UIContainer *root, SDL_Event *event, SDL_Renderer* renderer)
{
    static UIBase *ui_pressed = NULL;
    static UIBase *ui_hovered = NULL;
    static UIBase *ui_focused = NULL;
    (void)ui_focused;
    switch (event->type)
    {
    case SDL_EVENT_MOUSE_MOTION: {
        UIBase *hit = UIFindHit(root, event->motion.x, event->motion.y);
        if (hit != ui_hovered)
        {
	    if (ui_hovered && ui_hovered->on_mouse_exit)
		ui_hovered->on_mouse_exit(ui_hovered, renderer);
	    
	    ui_hovered = hit;
	    
	    if (ui_hovered && ui_hovered->on_mouse_enter)
		ui_hovered->on_mouse_enter(ui_hovered, renderer);
        }        
    }
    break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        UIBase *hit = UIFindHit(root, event->button.x, event->button.y);
	
        ui_pressed = hit;
        ui_focused = hit;        

        if (hit && hit->on_mouse_down)
            hit->on_mouse_down(hit, &event->button);        
	
        break;
    }

    case SDL_EVENT_MOUSE_BUTTON_UP: {
        UIBase *hit = UIFindHit(root, event->button.x, event->button.y);

        if (ui_pressed && ui_pressed->on_mouse_up)
            ui_pressed->on_mouse_up(ui_pressed, &event->button);
	
        if (ui_pressed && ui_pressed == hit) {
            if (ui_pressed->on_mouse_click)
                ui_pressed->on_mouse_click(ui_pressed, renderer);            
        }

        ui_pressed = NULL;
        break;
    }        
    }        
}


void
UIHandleEvent(UIContainer *root, SDL_Event *event, SDL_Renderer* renderer)
{
    switch (event->type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_MOTION:
        UIHandleMouse(root, event, renderer);
        break;

    default:
        break;
    }
}
