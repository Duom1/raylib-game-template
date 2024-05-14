#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#define TARGET_FPS 60

void updateDrawFrame(void);

typedef struct {
  Color wc;
  Color ac;
  Color sc;
  Color dc;
} Globals;

Globals globals;

int main(void) {
  InitWindow(800, 450, "wasd tester");

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(updateDrawFrame, TARGET_FPS, 1);
#else
  SetTargetFPS(TARGET_FPS);
  while (!WindowShouldClose()) {
    updateDrawFrame();
  }
#endif

  CloseWindow();

  return 0;
}

void updateDrawFrame(void) {
  globals.wc = GRAY;
  globals.ac = GRAY;
  globals.sc = GRAY;
  globals.dc = GRAY;

  if (IsKeyDown(KEY_W))
    globals.wc = GREEN;
  if (IsKeyDown(KEY_A))
    globals.ac = GREEN;
  if (IsKeyDown(KEY_S))
    globals.sc = GREEN;
  if (IsKeyDown(KEY_D))
    globals.dc = GREEN;

  BeginDrawing();
  ClearBackground(WHITE);
  int bo = 15;
  int s = 100;
  // clang-format off
  DrawRectangle(bo * 2 + s,      bo,          s, s, globals.wc);
  DrawRectangle(bo,              bo * 2 + s,  s, s, globals.ac);
  DrawRectangle(bo * 2 + s,      bo * 2 + s,  s, s, globals.sc);
  DrawRectangle(bo * 3 + s * 2,  bo * 2 + s,  s, s, globals.dc);
  // clang-format on
  EndDrawing();
}
