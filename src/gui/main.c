
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "app.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_button.h"
#include "recov_gui/ui_container.h"
#include "recov_gui/ui_event.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_window.h"
#include <stdio.h>

void
renderApp(App *app)
{
    UIBase *base = (UIBase *)app->window;
    base->on_update(base, app->window->renderer);
}

void
ScanButtonClickCB(UIBase *base)
{
    printf("Scanning Drive not implemented!\n");
    fflush(stdout);    
}

void
RecoverButtonClickCB(UIBase *base)
{
    printf("Recovering Files not implemented!\n");
    fflush(stdout);    
}    

int
main()
{
    App app = initApp("Recov", 800, 600);
    if (!UISetGlobalFont("JetBrainsMonoNerdFont-Regular.ttf", 14))
    {
	return 1;
    }
    UIWindow *window = app.window;
    UIButton *scan_btn = UIButtonInit(100, 50, 40, 40, "Scan");
    UIButton *recover_btn = UIButtonInit(100, 50, 40, 100, "Recover");
    
    UIBaseSetLayoutType(UIBASE(scan_btn), UI_LAYOUT_RELATIVE);
    UIBaseSetLayoutType(UIBASE(recover_btn), UI_LAYOUT_RELATIVE);
    
    UIContainerAddChild((UIContainer *)window, (UIBase *)scan_btn);
    UIContainerAddChild((UIContainer *)window, (UIBase *)recover_btn);
    
    UIButtonOnClick(scan_btn, ScanButtonClickCB);
    UIButtonOnClick(recover_btn, RecoverButtonClickCB);
    
    SDL_Event e;
    while (1)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                break;
            }
	    
            UIHandleEvent((UIContainer *)window, &e);
        }
        renderApp(&app);
        SDL_Delay(16);        
    }

    return 0;
}
