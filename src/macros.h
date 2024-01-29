#ifndef MACROS
#define MACROS

#include "globals.h"
#include "structs.h"

#define middle_X (float)SCREEN_W / 2
#define middle_Y (float)SCREEN_H / 2
#define pallet_middle_X (float)pallet_template.w / 2
#define pallet_middle_Y (float)pallet_template.h / 2

vec2 new_vec2(float x, float y);
SDL_Rect set_rect_pos(SDL_Rect r, vec2 v);
vec2 add_vec2(vec2 a, vec2 b);
vec2 sub_vec2(vec2 a, vec2 b);
vec2 mul_vec2(vec2 a, vec2 b);
vec2 div_vec2(vec2 a, vec2 b);

#endif