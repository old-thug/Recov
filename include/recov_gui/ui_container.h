#pragma once

#include "recov_gui/ui_base.h"
#include "recov_gui/ui_rect.h"
#include "shared/array.h"
#include <stdbool.h>

typedef arrayOf(UIBase *) UIChildren;

typedef struct UIContainer {
  UIBase base;
  UIChildren children;
} UIContainer;

UIContainer *UIContainerInit(size_t size, int64_t width, int64_t height,
                             int64_t xpos, int64_t ypos, UIComponentType type,
                             UIOnFocusFn on_focus, UIUpdateFn on_update,
                             void *user_data);
UIContainer *UIContainerInitRect(size_t size, UIRect rect, UIComponentType type,
                                 UIOnFocusFn on_focus, UIUpdateFn on_update,
                                 void *user_data);
void UIContainerAddChild(UIContainer *container, UIBase *child);
void UIContainerUpdateChildren(UIContainer *container, SDL_Renderer *renderer);
