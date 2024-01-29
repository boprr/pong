#ifndef DRAW
#define DRAW

#include <SDL2/SDL.h>

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY,
                int32_t radius);

void DrawDottedLine(SDL_Renderer *renderer, int x0, int y0, int x1, int y1);

#endif