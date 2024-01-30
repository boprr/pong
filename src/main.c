#include "draw.h"
#include "globals.h"
#include "macros.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_Window *win = SDL_CreateWindow("PONG", 0, 0, SCREEN_W, SCREEN_H, 0);
  SDL_Renderer *ren = SDL_CreateRenderer(win, 0, SDL_RENDERER_ACCELERATED);

  pallet player, bot;
  ball ball;

  SDL_Rect pallet_template;
  pallet_template.h = 150;
  pallet_template.w = 15;
  pallet_template.y = middle_Y;

  player.rect = pallet_template;
  player.pos.x = pallet_template.w + (pallet_middle_X);
  player.pos.y = pallet_template.y;

  bot.rect = pallet_template;
  bot.pos.x = SCREEN_W - pallet_template.w - (pallet_middle_X);
  bot.pos.y = pallet_template.y;

  ball.col.h = 20;
  ball.col.w = 20;
  ball.pos = new_vec2(middle_X, middle_Y);
  ball.vel.x = -0.2;

  SDL_Event event;
  const Uint8 *key = SDL_GetKeyboardState(NULL);

  Uint64 dt_now = SDL_GetPerformanceCounter();
  Uint64 dt_last = 0;
  double dt = 0;

  int score = 0;

  TTF_Font *font = TTF_OpenFont("assets/font/Terminus.ttf", 100);
  SDL_Color White = {255, 255, 255};

  SDL_Rect score_rect;
  score_rect.h = 100;
  score_rect.w = 100;
  score_rect.x = 0;
  score_rect.y = 0;

  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);
    key = SDL_GetKeyboardState(NULL);

    // -- Delta Time -- //
    dt_last = dt_now;
    dt_now = SDL_GetPerformanceCounter();
    dt = (double)(dt_now - dt_last) * 1000 /
         (double)SDL_GetPerformanceFrequency();

    // -- Logic -- //

    player.d_last = player.d_new;
    bot.d_last = bot.d_new;
    player.d_new = player.pos;
    bot.d_new = bot.pos;
    player.delta = sub_vec2(player.d_new, player.d_last);
    bot.delta = sub_vec2(bot.d_new, bot.d_last);

    if (key[SDL_SCANCODE_W]) {
      player.pos.y -= 0.25 * dt;
    }
    if (key[SDL_SCANCODE_S]) {
      player.pos.y += 0.25 * dt;
    }

    // bot ai
    if (bot.pos.y > ball.pos.y) {
      bot.pos.y -= 0.25 * dt;
    }
    if (bot.pos.y < ball.pos.y) {
      bot.pos.y += 0.25 * dt;
    }

    if (player.pos.y < pallet_middle_Y) {
      player.pos.y = pallet_middle_Y;
    }
    if (player.pos.y > SCREEN_H - pallet_middle_Y) {
      player.pos.y = SCREEN_H - pallet_middle_Y;
    }

    if (bot.pos.y < pallet_middle_Y) {
      bot.pos.y = pallet_middle_Y;
    }
    if (bot.pos.y > SCREEN_H - pallet_middle_Y) {
      bot.pos.y = SCREEN_H - pallet_middle_Y;
    }

    if (SDL_HasIntersection(&player.rect, &ball.col)) {
      ball.vel.x *= -1;
      ball.pos.x += 1;
      ball.vel.y += player.delta.y;
      ball.pos.y += 0.1;
      ball.vel.x *= 1.1f;
    }
    if (SDL_HasIntersection(&bot.rect, &ball.col)) {
      ball.vel.x *= -1;
      ball.pos.x -= 1;
      ball.vel.y += bot.delta.y;
      ball.pos.y -= 0.1;
      ball.vel.x *= 1.1f;
    }

    if ((unsigned int)ball.vel.x >= 0.6f) {
      if ((unsigned int)ball.vel.x != ball.vel.x) {
        ball.vel.x = -0.6;
      } else {
        ball.vel.x = 0.6;
      }
    }
    // wtf

    if (ball.pos.y < 0 + (float)ball.col.h / 2 ||
        ball.pos.y > SCREEN_H - (float)ball.col.h / 2) {
      ball.vel.y *= -1;
    }

    if (ball.pos.x < 0) {
      score--;
      ball.pos = new_vec2(middle_X, middle_Y);
      ball.vel.x = -0.3;
      player.pos.y = middle_Y;
      ball.vel.y = 0;
    }
    if (ball.pos.x > SCREEN_W) {
      score++;
      ball.pos = new_vec2(middle_X, middle_Y);
      ball.vel.y = 0;
      ball.vel.x = 0.2;
    }

    // printf("FPS: %o\n", SDL_GetPerformanceFrequency() / (dt_now - dt_last));

    ball.pos.x += ball.vel.x * dt;
    ball.pos.y += ball.vel.y * dt;

    // -- Font -- //
    char score_str[4];
    sprintf(score_str, "%i", score);

    SDL_Surface *surfaceScore = TTF_RenderText_Solid(font, score_str, White);
    SDL_Texture *ScoreSurface = SDL_CreateTextureFromSurface(ren, surfaceScore);
    TTF_SizeText(font, score_str, &score_rect.w, &score_rect.h);

    score_rect.x = SCREEN_W / 2 - (score_rect.w / 2);

    // -- Setting vars -- //
    ball.col = set_rect_pos(ball.col, ball.pos);
    player.rect = set_rect_pos(player.rect, player.pos);
    bot.rect = set_rect_pos(bot.rect, bot.pos);

    // -- Rendering -- //
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // FG

    SDL_RenderDrawRect(ren, &bot.rect);
    SDL_RenderDrawRect(ren, &player.rect);
    DrawDottedLine(ren, SCREEN_W / 2, 0, SCREEN_W / 2, SCREEN_H);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderFillRect(ren, &score_rect);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // FG
    DrawCircle(ren, ball.pos.x, ball.pos.y, ball.col.h / 2);
    SDL_RenderCopy(ren, ScoreSurface, NULL, &score_rect);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderPresent(ren);

    SDL_FreeSurface(surfaceScore);
    SDL_DestroyTexture(ScoreSurface);
  }

  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  return 0;
}

/*
--- TODO ---
1. fix collisions on 10-200 fps
2. audio
*/
