#include "globals.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#define DEBUG 0

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY,
                int32_t radius) {
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

void DrawDottedLine(SDL_Renderer *renderer, int x0, int y0, int x1, int y1) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;
  int count = 0;
  while (1) {
    if (count < 10) {
      SDL_RenderDrawPoint(renderer, x0, y0);
    }
    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 > dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
    count = (count + 1) % 20;
  }
}

int main() {
  SDL_Window *win = SDL_CreateWindow("PONG", 0, 0, SCREEN_W, SCREEN_H, 0);
  SDL_Renderer *ren = SDL_CreateRenderer(win, 0, 0);

  struct pallet player;
  struct pallet bot;
  struct ball ball;

  TTF_Init();

  TTF_Font *font;
  font = TTF_OpenFont("upheavtt.ttf", 24);
  SDL_Surface *text;
  SDL_Color color = {0, 255, 0};

  SDL_Texture *text_texture;

  SDL_Rect pallet_template;
  pallet_template.h = 150;
  pallet_template.w = 15;
  pallet_template.y = (SCREEN_H / 2) - (pallet_template.h / 2);

  SDL_Rect text_rect;
  text_rect.w = 100;
  text_rect.h = 100;
  text_rect.x = 50;
  text_rect.y = 50;

  player.rect = pallet_template;
  player.pos.x = 15;
  player.pos.y = 0;
  float player_speed = 0.25;
  bot.rect = pallet_template;
  bot.pos.x = SCREEN_W - (15 * 2);

  ball.col.h = 20;
  ball.col.w = 20;
  ball.pos = new_vec2(((float)SCREEN_W / 2) - ((float)ball.col.w / 2),
                      (float)SCREEN_H / 2 - ((float)ball.col.w / 2) -
                          ((float)ball.col.h / 2));

  SDL_Event event;

  ball.vel.x = -0.2;

  float pl_dt_last, pl_dt_new, pl_dt;
  float bt_dt_last, bt_dt_new, bt_dt;

  Uint64 dt_now = SDL_GetPerformanceCounter();
  Uint64 dt_last = 0;
  double dt = 0;

  const Uint8 *key = SDL_GetKeyboardState(NULL);

  int score = 0;

  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);
    key = SDL_GetKeyboardState(NULL);

    // -- Delta Time -- //
    dt_last = dt_now;
    dt_now = SDL_GetPerformanceCounter();
    dt = (double)(dt_now - dt_last) * 1000 /
         (double)SDL_GetPerformanceFrequency();

    // -- Logic -- //
    char score_str[4];
    sprintf(score_str, "%d", score);
    text = TTF_RenderText_Solid(font, score_str, color);
    text_texture = SDL_CreateTextureFromSurface(ren, text);
    SDL_Rect dest = {0, 0, text->w, text->h};

    pl_dt_last = pl_dt_new;
    pl_dt_new = player.pos.y;

    bt_dt_last = bt_dt_new;
    bt_dt_new = bot.pos.y;

    pl_dt = pl_dt_new - pl_dt_last;
    bt_dt = bt_dt_new - bt_dt_last;

    if (key[SDL_SCANCODE_W]) {
      player.pos.y -= player_speed * dt;
    }
    if (key[SDL_SCANCODE_S]) {
      player.pos.y += player_speed * dt;
    }

    ball.pos.x += ball.vel.x * dt;
    ball.pos.y += ball.vel.y * dt;

    if (SDL_HasIntersection(&player.rect, &ball.col)) {
      ball.vel.x *= -1;
      ball.pos.x += 0.1;
      ball.vel.y += pl_dt;
      ball.pos.y += 0.1;
      ball.vel.x *= 1.1f;
    }
    if (SDL_HasIntersection(&bot.rect, &ball.col)) {
      ball.vel.x *= -1;
      ball.pos.x -= 0.1;
      ball.vel.y -= bt_dt;
      ball.pos.y -= 0.1;
      ball.vel.x *= 1.1f;
    }

    // if (ball.vel.x <= 0.5) {
    // ball.vel.x = 0.5;
    //}

    // printf("%f\n", ball.vel.x);

    if (ball.pos.y > SCREEN_H - ball.col.h) {
      ball.vel.y *= -1;
      ball.pos.y = SCREEN_H - ball.col.h;
    }
    if (ball.pos.y < 0) {
      ball.vel.y *= -1;
      ball.pos.y = 0;
    }

    if (ball.pos.y < bot.pos.y + ((float)bot.rect.h / 2)) {
      bot.pos.y -= player_speed * dt;
    }

    if (ball.pos.y > bot.pos.y + ((float)bot.rect.h / 2)) {
      bot.pos.y += player_speed * dt;
    }

    // bot.pos.y = ball.pos.y - (bot.pos.y / 2); // AI

    if (player.pos.y > SCREEN_H - player.rect.h) {
      player.pos.y = SCREEN_H - player.rect.h;
    }
    if (player.pos.y < 0) {
      player.pos.y = 0;
    }

    if (bot.pos.y > SCREEN_H - bot.rect.h) {
      bot.pos.y = SCREEN_H - bot.rect.h;
    }
    if (bot.pos.y < 0) {
      bot.pos.y = 0;
    }

    if (ball.pos.x < 0) {
      score--;
      ball.pos = new_vec2(((float)SCREEN_W / 2) - ((float)ball.col.w / 2),
                          (float)SCREEN_H / 2 - ((float)ball.col.w / 2) -
                              ((float)ball.col.h / 2));
      ball.vel.x *= -1;
      ball.vel.y = 0;
    }
    if (ball.pos.x > SCREEN_W) {
      score++;
      ball.pos = new_vec2(((float)SCREEN_W / 2) - ((float)ball.col.w / 2),
                          (float)SCREEN_H / 2 - ((float)ball.col.w / 2) -
                              ((float)ball.col.h / 2));
      ball.vel.y = 0;
      ball.vel.x = 0.1;
    }

    if (ball.pos.x > 1000) {
      // ball out of bounds
      ball.pos.x = SCREEN_W / 2;
      ball.vel.x = 0.1;
    }

    if (ball.pos.y > 1000) {
      // ball out of bounds
      ball.pos.y = SCREEN_H / 2;
      ball.vel.y = 0.1;
    }

    // printf("pos: %f, %f \n", player.pos.x, player.pos.y);

    // -- Setting vars -- //

    ball.col = set_rect_pos(ball.col, ball.pos);
    player.rect = set_rect_pos(player.rect, player.pos);
    bot.rect = set_rect_pos(bot.rect, bot.pos);

    // -- Rendering -- //
    SDL_RenderClear(ren);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // FG

    SDL_RenderDrawRect(ren, &bot.rect);
    SDL_RenderDrawRect(ren, &player.rect);
    DrawDottedLine(ren, SCREEN_W / 2, 0, SCREEN_W / 2, SCREEN_H);
    DrawCircle(ren, ball.pos.x + ((float)ball.col.w / 2),
               ball.pos.y + ((float)ball.col.h / 2), 11);
    SDL_RenderCopy(ren, text_texture, &text_rect, &dest);
#if DEBUG
    SDL_RenderDrawRect(ren, &ball.col);
#endif

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // BG
    SDL_RenderPresent(ren);
  }

  SDL_DestroyWindow(win);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  return 0;
}