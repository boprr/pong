#include "macros.h"
#include "structs.h"
#include <SDL2/SDL_rect.h>

vec2 new_vec2(float x, float y) {
  vec2 v;
  v.x = x;
  v.y = y;
  return v;
}
vec2 add_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x + b.x;
  v.y = a.y + b.y;
  return v;
}
vec2 sub_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x - b.x;
  v.y = a.y - b.y;
  return v;
}
vec2 mul_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x * b.x;
  v.y = a.y * b.y;
  return v;
}
vec2 div_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x / b.x;
  v.y = a.y / b.y;
  return v;
}
SDL_Rect set_rect_pos(SDL_Rect r, vec2 v) {
  r.x = v.x - ((float)r.w / 2);

  r.y = v.y - ((float)r.h / 2);
  return r;
}