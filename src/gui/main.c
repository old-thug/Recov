
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "app.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_button.h"
#include "recov_gui/ui_container.h"
#include "recov_gui/ui_event.h"
#include "recov_gui/ui_label.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_panel.h"
#include "recov_gui/ui_window.h"
#include <stdio.h>

void
renderApp(App *app)
{
    UIBase *base = (UIBase *)app->window;
    base->on_update(base, app->window->renderer);
}

void
ScanButtonClickCB(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;
    (void)renderer;    
    printf("Scanning Drive not implemented!\n");
    fflush(stdout);
}

void
ExitButtonClickCB(UIBase *base, SDL_Renderer* renderer)
{
    (void)base;
    (void)renderer;    
    // TODO: clean up.
    exit(0);
}

// For Custom Styles
UIButton *
MakeButton(int w, int h, int x, int y, const char *text)
{
    UIButton *btn = UIButtonInit(w, h, x, y, text);
    UIBaseSetBackGround(UIBASE(btn), UIColorInit(0, 122, 204, 255));
    return btn;
}

int
main()
{
    App app = initApp("Recov", 800, 600);
    if (!UISetGlobalFont("JetBrainsMonoNerdFont-Regular.ttf", 16))
    {
        return 1;
    }

    UIWindow *window = app.window;
    UIBaseSetBackGround(UIBASE(window), UIColorInit(30, 30, 30, 255));

    UILabel *title = UILabelInit(200, 100, 300, 10, "C:/Recov");
    
    UIPanel *main_panel = UIPanelInit(800, 500, 0, 50);
    UIButton *scan_btn = MakeButton(300, 50, 250, 190, "Scan");

    UIButton *exit_btn = MakeButton(300, 50, 250, 260, "Exit");
    UIButtonOnClick(exit_btn, ExitButtonClickCB);
    
    UIBaseSetBackGround(UIBASE(main_panel), UIColorInit(37, 37, 38, 255));
    
    UIBaseSetLayoutType(UIBASE(main_panel), UI_LAYOUT_RELATIVE);

    UIContainerAddChild((UIContainer *)main_panel, UIBASE(scan_btn));
    UIContainerAddChild((UIContainer *)main_panel, UIBASE(exit_btn));
    UIContainerAddChild((UIContainer *)main_panel, UIBASE(title));

    UIContainerAddChild((UIContainer *)window, UIBASE(main_panel));

    SDL_Event e;
    while (1)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                break;
            }

            UIHandleEvent((UIContainer *)window, &e, window->renderer);
        }
        renderApp(&app);
        SDL_Delay(16);
    }

    return 0;
}
