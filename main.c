#include <raylib.h>
#include <stdio.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#define TARGET_FPS 60

typedef struct {
  Image testImage;
  Texture2D texture;
  int height;
  int width;
  int ogHeight;
  int ogWidth;
} Globals;

Globals globals;

void updateDrawFrame(void) {
  static Color wc;
  static Color ac;
  static Color sc;
  static Color dc;
  static char wsize[20];
  wc = GRAY;
  ac = GRAY;
  sc = GRAY;
  dc = GRAY;

  if (IsWindowResized()) {
    globals.height = GetScreenHeight();
    float scale = (float)globals.height / (float)globals.ogHeight;
    globals.width = (int)((float)globals.ogWidth * scale);
    sprintf(wsize, "%i %i", globals.width, globals.height);
    SetWindowSize(globals.width, globals.height);
  }

  if (IsKeyDown(KEY_W))
    wc = GREEN;
  if (IsKeyDown(KEY_A))
    ac = GREEN;
  if (IsKeyDown(KEY_S))
    sc = GREEN;
  if (IsKeyDown(KEY_D))
    dc = GREEN;

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
  DrawTexture(globals.texture, globals.width - globals.texture.width,
              globals.height - globals.texture.height, WHITE);
  DrawText(wsize, 10, 10, 20, RED);
  EndDrawing();
}

int main(void) {
  globals.width = globals.ogWidth = 1920 / 2;
  globals.height = globals.ogHeight = 1080 / 2;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(globals.width, globals.height, "wasd tester");

  globals.testImage = LoadImage("./res/test.png");
  ImageResize(&globals.testImage, globals.testImage.width / 3,
              globals.testImage.height / 3);
  globals.texture = LoadTextureFromImage(globals.testImage);

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(updateDrawFrame, TARGET_FPS, 1);
#else
  SetTargetFPS(TARGET_FPS);
  while (!WindowShouldClose()) {
    updateDrawFrame();
  }
#endif

  UnloadImage(globals.testImage);
  UnloadTexture(globals.texture);
  CloseWindow();

  return 0;
}
