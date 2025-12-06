#pragma once

#include "recov_gui/ui_container.h"
#include "recov_gui/ui_layout.h"

typedef struct UIPanel {
    UIContainer base;
    UILayoutType layout;
} UIPanel;

UIPanel *UIPanelInit(int64_t width, int64_t height, int64_t xpos, int64_t ypos);
void UIPanelSetLayout(UIPanel *panel, UILayoutType layout);
