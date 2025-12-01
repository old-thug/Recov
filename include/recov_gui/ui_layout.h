#pragma once


typedef enum
{
    UI_LAYOUT_ABSOLUTE,
    UI_LAYOUT_RELATIVE,
    UI_LAYOUT_ANCHORED,
    UI_LAYOUT_VERTICAL_STACK,
    UI_LAYOUT_HORIZONTAL_STACK
} UILayoutType;

typedef struct 
{
    float x, y;
    float w, h;
} __BaseLayout;

typedef __BaseLayout UILayoutAbsolute;
typedef __BaseLayout UILayoutRelative;

typedef struct UILayout
{
    UILayoutType type;
    union
    {
        UILayoutRelative relative;
        UILayoutAbsolute absolute;        
    };        
} UILayout;


UILayout UILayoutAbsoluteInit (float x, float y,
                                       float w, float h);
UILayout UILayoutRelativeInit (float x, float y,
                                       float w, float h);


