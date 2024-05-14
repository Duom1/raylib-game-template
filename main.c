#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

void updateDrawFrame(void);

int main(void) {
  InitWindow(800, 450, "raylib [core] example - basic window");

#ifdef PLATFORM_WEB

  emscripten_set_main_loop(updateDrawFrame, 60, 1);

#else

  while (!WindowShouldClose()) {
    updateDrawFrame();
  }

#endif

  CloseWindow();

  return 0;
}

void updateDrawFrame(void) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
  EndDrawing();
}
