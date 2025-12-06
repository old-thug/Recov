#pragma once

#include "SDL3/SDL_rect.h"
#include <stddef.h>
#include <stdint.h>

typedef struct UIRect {
  int64_t w;
  int64_t h;
  int64_t x;
  int64_t y;
} UIRect;

UIRect UIRectInit(int64_t width, int64_t height, int64_t xpos, int64_t ypos);
SDL_FRect UIRectToFRect(UIRect rect);
