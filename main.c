#include <raylib.h>
#include <stdio.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#define TARGET_FPS 60

typedef struct {
  Texture2D swirl;
  Texture2D texture;
  int height;
  int width;
  int ogHeight;
  int ogWidth;
  float scale;
} Globals;

Globals globals;

void updateDrawFrame(void) {
  static Color wc;
  static Color ac;
  static Color sc;
  static Color dc;
  static char wsize[20];
#ifndef PLATFORM_WEB
  static bool tfull = false;
#endif /* ifndef PLATFORM_WEB */
  // related to the swirl animation
  static int swirlIndex = 0;
  static double animFps = 1.0 / 16.0;
  static double swirlScale = 10.0;
  static double prevSwi = 0;

  wc = GRAY;
  ac = GRAY;
  sc = GRAY;
  dc = GRAY;

#ifndef PLATFORM_WEB
  if (tfull) {
    ToggleFullscreen();
    tfull = false;
  }
  if (IsKeyPressed(KEY_F11)) {
    globals.width = 1920;
    globals.height = 1080;
    SetWindowSize(globals.width, globals.height);
    tfull = !tfull;
  }
#endif /* ifndef PLATFORM_WEB */
  if (IsWindowResized()) {
    globals.height = GetScreenHeight();
    globals.scale = (float)globals.height / (float)globals.ogHeight;
    globals.width = (int)((float)globals.ogWidth * globals.scale);
    sprintf(wsize, "%i %i", globals.width, globals.height);
  }
  if (IsKeyPressed(KEY_ZERO) && IsKeyDown(KEY_LEFT_CONTROL)) {
    SetWindowSize(globals.width, globals.height);
  }

  double curTime = GetTime();
  if (curTime - prevSwi > animFps) {
    prevSwi = curTime;
    ++swirlIndex;
    if (swirlIndex >= 15)
      swirlIndex = 0;
  }

  if (IsKeyDown(KEY_W))
    wc = GREEN;
  if (IsKeyDown(KEY_A))
    ac = GREEN;
  if (IsKeyDown(KEY_S))
    sc = GREEN;
  if (IsKeyDown(KEY_D))
    dc = GREEN;

  // DRAWING LOOP ====
  BeginDrawing();
  ClearBackground(RAYWHITE);
  static const int bo = 15;
  static const int s = 100;
  // clang-format off
  DrawRectangle(bo * 2 + s,      bo,          s, s, wc);
  DrawRectangle(bo,              bo * 2 + s,  s, s, ac);
  DrawRectangle(bo * 2 + s,      bo * 2 + s,  s, s, sc);
  DrawRectangle(bo * 3 + s * 2,  bo * 2 + s,  s, s, dc);
  // clang-format on
  DrawTextureEx(
      globals.texture,
      (Vector2){globals.width - globals.texture.width * (globals.scale * 0.2),
                globals.height -
                    globals.texture.height * (globals.scale * 0.2)},
      0, globals.scale * 0.2, WHITE);
  DrawText(wsize, 10, 10, 20, RED);
  DrawTexturePro(globals.swirl, (Rectangle){0, 16 * swirlIndex, 16, 16},
                 (Rectangle){0,
                             globals.height - (16 * swirlScale * globals.scale),
                             16 * swirlScale * globals.scale,
                             16 * swirlScale * globals.scale},
                 (Vector2){0, 0}, 0, WHITE);
  EndDrawing();
}

int main(void) {
  globals.width = globals.ogWidth = 1920 / 2;
  globals.height = globals.ogHeight = 1080 / 2;
  globals.scale = 1.0;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(globals.width, globals.height, "wasd tester");

  globals.texture = LoadTexture("./res/test.png");
  globals.swirl = LoadTexture("./res/swirl.png");

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(updateDrawFrame, TARGET_FPS, 1);
#else
  SetTargetFPS(TARGET_FPS);
  while (!WindowShouldClose()) {
    updateDrawFrame();
  }
#endif

  UnloadTexture(globals.texture);
  UnloadTexture(globals.swirl);
  CloseWindow();

  return 0;
}
