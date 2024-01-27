
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
#include <stdio.h>

#define DEBUG 1

int main() {
  SDL_Window *win = SDL_CreateWindow("PONG", 0, 0, SCREEN_W, SCREEN_H, 0);
  SDL_Renderer *ren = SDL_CreateRenderer(win, 0, 0);

  struct pallet player;
  struct pallet bot;
  struct ball ball;

  SDL_Rect pallet_template;
  pallet_template.h = 150;
  pallet_template.w = 15;
  pallet_template.y = (SCREEN_H / 2) - (pallet_template.h / 2);

  player.rect = pallet_template;
  player.pos.x = 15;
  float player_speed = 0.5;
  bot.rect = pallet_template;
  bot.pos.x = SCREEN_W - (15 * 2);

  ball.col.h = 10;
  ball.col.w = 10;
  ball.pos = new_vec2(((float)SCREEN_W / 2) - ((float)ball.col.w / 2),
                      (float)SCREEN_H / 2 - ((float)ball.col.w / 2) -
                          ((float)ball.col.h / 2));

  SDL_Event event;

  ball.vel.x = -0.1;

  float pl_dt_last, pl_dt_new, pl_dt;
  float bt_dt_last, bt_dt_new, bt_dt;

  Uint64 dt_now = SDL_GetPerformanceCounter();
  Uint64 dt_last = 0;
  double dt = 0;

  const Uint8 *key = SDL_GetKeyboardState(NULL);

  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);
    key = SDL_GetKeyboardState(NULL);

    // -- Delta Time -- //
    dt_last = dt_now;
    dt_now = SDL_GetPerformanceCounter();
    dt = (double)(dt_now - dt_last) * 1000 /
         (double)SDL_GetPerformanceFrequency();

    // -- Logic -- //

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
    }
    if (SDL_HasIntersection(&bot.rect, &ball.col)) {
      ball.vel.x *= -1;
      ball.pos.x -= 0.1;
      ball.vel.y -= bt_dt;
      ball.pos.y -= 0.1;
    }

    printf("x:%f, y:%f \n", ball.pos.y, ball.pos.x);

    if (ball.pos.y > SCREEN_H - ((float)ball.col.h / 2) ||
        ball.pos.y < ball.col.h / 2) {
      ball.vel.y *= -1;
      ball.vel.y -= 0.1;
    }

    bot.pos.y = ball.pos.y - (bot.pos.y / 2); // AI

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
#if DEBUG
    SDL_RenderDrawRect(ren, &ball.col);
#endif

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // BG
    SDL_RenderPresent(ren);
  }

  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  return 0;
}