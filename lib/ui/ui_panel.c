#include "recov_gui/ui_panel.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_container.h"
#include "recov_gui/ui_layout.h"
#include "recov_gui/ui_rect.h"

UIPanel *
UIPanelInit(int64_t width, int64_t height, int64_t xpos, int64_t ypos)
{
    UIPanel *panel =
        (UIPanel *)UIContainerInit(sizeof(UIPanel), width, height, xpos, ypos,
                                   UICOMP_Panel, NULL, NULL, NULL);
    return panel;    
}

void
UIPanelSetLayout(UIPanel *panel, UILayoutType layout)
{
    panel->base.base.layout = layout;    
}    
