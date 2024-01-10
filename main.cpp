#include <cstdio> // imports printf
#include "raylib.h"
#include "raymath.h"

#define GLSL_VERSION 330

class EventLoop {
  public:
    Shader bgShader;
    Font font;
    Color primaryColor;
    void main(); // main event loop
};

void EventLoop::main() {
  // --- UPDATE STATE ---
  // get world parameters
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  int fps = GetFPS();
  double elapsed = GetTime();
  Vector2 mousePos = GetMousePosition();
  Vector2 screenCenter = { (float)w/2, (float)h/2 };

  // calculate triangle position and rotation
  Vector2 relPos = Vector2Subtract(mousePos, screenCenter);
  Vector2 normalizedPos = Vector2Normalize(relPos);
  normalizedPos.x = 65.0 * normalizedPos.x;
  normalizedPos.y = 65.0 * normalizedPos.y;
  Vector2 absPos = Vector2Add(screenCenter, normalizedPos);
  float relAngle = atan2(relPos.y, relPos.x);

  // add uniforms to background shader
  float screenSize[2] = { (float)w, (float)h };
  float t = (float)elapsed;
  SetShaderValue(bgShader, GetShaderLocation(bgShader, "t"), &t, SHADER_UNIFORM_FLOAT);
  SetShaderValue(bgShader, GetShaderLocation(bgShader, "screen_size"), &screenSize, SHADER_UNIFORM_VEC2);

  // --- DRAW TO SCREEN ---
  BeginDrawing();
    ClearBackground(BLACK);
    if (IsWindowFocused()) {
      // draw background
      BeginShaderMode(bgShader);
        DrawRectangle(0, 0, w, h, WHITE);
      EndShaderMode();
      
      // draw rotating hex
      DrawPoly(screenCenter, 6, 50, elapsed * 10, primaryColor);
      // draw triangle
      DrawPoly(absPos, 3, 10, relAngle * 180.0 / PI, primaryColor);
      // debug line
      // DrawLine(w/2, h/2, (int)mousePos.x, (int)mousePos.y, GREEN);

      // draw text overlay
      DrawTextEx(font, TextFormat("FPS: %i", fps), (Vector2){ 10, 10 }, 20, 3.5, WHITE);
      DrawTextEx(font, TextFormat("Duration: %f", elapsed), (Vector2){ 10, (float)h - 30 }, 20, 3.5, WHITE);

    } else {
      DrawText("Pay attention to me", 10, 10, 24, RED);
    }
  EndDrawing();
}

int main() {
  // --- INITIALIZATION ---
  // SetWindowIcon(image);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Window configuration flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(800, 600, "Test Window");
  SetWindowMinSize(400, 300);
  SetTargetFPS(120);

  // TTF font : Font data and atlas are generated directly from TTF
  Font fontRetro = LoadFontEx("assets/retro_computer.ttf", 32, 0, 0);
  SetTextLineSpacing(48);

  Shader shader = LoadShader(0, TextFormat("assets/test.frag", GLSL_VERSION));
  Color primary = GetColor(0xafaaecff);

  // add assets to event loop
  EventLoop eventLoop;
  eventLoop.bgShader = shader;
  eventLoop.font = fontRetro;
  eventLoop.primaryColor = primary;
  
  // --- EVENT LOOP ---
  printf("\n\n\n-- Starting Event Loop --\n");
  while (!WindowShouldClose()) {
    eventLoop.main();
  }

  // --- CLEAN UP ---
  UnloadShader(shader);
  UnloadFont(fontRetro);
  CloseWindow();

  return 0;
}