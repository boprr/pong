#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

typedef struct {
  float x;
  float y;
} vec2;

vec2 new_vec2(float x, float y) {
  vec2 v;
  v.x = x;
  v.y = y;

  return v;
}

SDL_Rect set_rect_pos(SDL_Rect r, vec2 v) {
  r.x = v.x;
  r.y = v.y;
  return r;
}

struct pallet {
  SDL_Rect rect;
  vec2 pos;
};

struct ball {
  SDL_Rect col;
  vec2 pos;
  vec2 vel;
};

#endif