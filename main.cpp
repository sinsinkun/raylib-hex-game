#include <cstdio> // imports printf
#include "raylib.h"

#define GLSL_VERSION 330

// main event loop
void EventLoop(Shader shader) {
  ClearBackground(BLACK);
  if (IsWindowFocused()) {
    // get world parameters
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    int fps = GetFPS();
    double elapsed = GetTime();
    // Vector2 mousePos = GetMousePosition();

    // add uniforms to shader
    float screenSize[2] = { (float)w, (float)h };
    float t = (float)elapsed;
    SetShaderValue(shader, GetShaderLocation(shader, "t"), &t, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, GetShaderLocation(shader, "screen_size"), &screenSize, SHADER_UNIFORM_VEC2);
    // draw background
    BeginShaderMode(shader);
      DrawRectangle(0, 0, w, h, WHITE);
    EndShaderMode();

    // printf("Debug - %i %f\n", fps, elapsed);
    DrawText(TextFormat("%i %f", fps, elapsed), 10, 10, 18, WHITE);
    DrawPoly((Vector2){ (float)w/2, (float)h/2 }, 4, 80, elapsed * 10, WHITE);

  } else {
    DrawText("Pay attention to me", 10, 10, 24, RED);
  }
}

int main() {
  // --- INITIALIZATION ---
  // SetWindowIcon(image);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Window configuration flags
  InitWindow(800, 600, "Test Window");
  SetWindowMinSize(400, 300);
  SetTargetFPS(120);

  Shader shader = LoadShader(0, TextFormat("assets/test.frag", GLSL_VERSION));
  
  // --- EVENT LOOP ---
  printf("\n\n\n-- Starting Event Loop --\n");
  while (!WindowShouldClose()) {
    BeginDrawing();
      EventLoop(shader);
    EndDrawing();
  }

  // --- CLEAN UP ---
  UnloadShader(shader);
  CloseWindow();

  return 0;
}