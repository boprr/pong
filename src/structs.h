#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL_rect.h>

typedef struct {
  float x;
  float y;
} vec2;

typedef struct {
  SDL_Rect rect;
  vec2 pos;
  vec2 delta, d_last, d_new;
} pallet;

typedef struct {
  SDL_Rect col;
  vec2 pos;
  vec2 vel;
} ball;

#endif