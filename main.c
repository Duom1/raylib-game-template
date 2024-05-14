#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#define TARGET_FPS 60

typedef struct {
  Image testImage;
  Texture2D texture;
  int height;
  int width;
} Globals;

Globals globals;

void updateDrawFrame(void) {
  static Color wc;
  static Color ac;
  static Color sc;
  static Color dc;
  wc = GRAY;
  ac = GRAY;
  sc = GRAY;
  dc = GRAY;

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
  static int bo = 15;
  static int s = 100;
  // clang-format off
  DrawRectangle(bo * 2 + s,      bo,          s, s, wc);
  DrawRectangle(bo,              bo * 2 + s,  s, s, ac);
  DrawRectangle(bo * 2 + s,      bo * 2 + s,  s, s, sc);
  DrawRectangle(bo * 3 + s * 2,  bo * 2 + s,  s, s, dc);
  // clang-format on
  DrawTexture(globals.texture, globals.width - globals.texture.width,
              globals.height - globals.texture.height, WHITE);
  EndDrawing();
}

int main(void) {
  globals.width = 800;
  globals.height = 600;
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
