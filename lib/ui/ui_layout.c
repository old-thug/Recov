#include "recov_gui/ui_layout.h"


UILayout
UILayoutAbsoluteInit (float x, float y, float w, float h)
{
    return (UILayout){ .type = UI_LAYOUT_ABSOLUTE,
                       .absolute = { .x = x, .y = y, .w = w, .h = h } };    
}

UILayout
UILayoutRelativeInit (float x, float y, float w, float h)
{
    return (UILayout){ .type = UI_LAYOUT_RELATIVE,
	.relative = { .x = x, .y = y, .w = w, .h = h } };    
}

