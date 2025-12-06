#pragma once

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_ttf.h"
#include "SDL3/SDL_video.h"
#include "recov_gui/ui_base.h"
#include "recov_gui/ui_rect.h"
#include "recov_gui/ui_window.h"

typedef struct App {
  UIWindow *window;
} App;

static inline App initApp(const char *title, int width, int height) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  App app;
  app.window = UIWindowInit(width, height, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, title, NULL);
  UIBaseSetBackGround(UIBASE(app.window), UIColorInit(51, 111, 221, 255));
  return app;
}
