#include <cstdio> // imports printf
#include "raylib.h"
#include "raymath.h"
#include "wallShape.h"

#define GLSL_VERSION 330

enum Scene {
  menu,
  game
};

class EventLoop {
  public:
    Shader bgShader;
    Font font;
    Color primaryColor;
    Scene scene;
    void root(); // main event loop
    void menu();
    void game();
};

void EventLoop::root() {
  switch(scene) {
    case Scene::game:
      game();
      break;
    case Scene::menu:
    default:
      menu();
      break;
  }
}

void EventLoop::menu() {
  // --- UPDATE STATE ---
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  int fps = GetFPS();
  double elapsed = GetTime();
  // Vector2 mousePos = GetMousePosition();
  // Vector2 screenCenter = { (float)w/2, (float)h/2 };

  // add uniforms to background shader
  float screenSize[2] = { (float)w, (float)h };
  float t = (float)elapsed;
  SetShaderValue(bgShader, GetShaderLocation(bgShader, "t"), &t, SHADER_UNIFORM_FLOAT);
  SetShaderValue(bgShader, GetShaderLocation(bgShader, "screen_size"), &screenSize, SHADER_UNIFORM_VEC2);

  // --- DRAW TO SCREEN ---
  BeginDrawing();
    ClearBackground(BLACK);
    // draw background
    BeginShaderMode(bgShader);
      DrawRectangle(0, 0, w, h, WHITE);
    EndShaderMode();

    // draw text overlay
    DrawTextEx(font, TextFormat("FPS: %i", fps), (Vector2){ 10, 10 }, 20, 3.5, WHITE);
    DrawTextEx(font, TextFormat("Duration: %f", elapsed), (Vector2){ 10, (float)h - 30 }, 20, 3.5, WHITE);
    DrawTextEx(font, "Hit space to enter game", (Vector2){ (float)w/2 - 170, (float)h/2 - 10 }, 20, 3.5, WHITE);
  EndDrawing();

  // --- REGISTER INPUT ---
  if (IsKeyPressed(KEY_SPACE)) scene = Scene::game;
}

void EventLoop::game() {
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
      WallShape::DrawWall(screenCenter, 80, 25, elapsed * 10, RED);

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
  eventLoop.scene = Scene::menu;
  
  // --- EVENT LOOP ---
  printf("\n\n\n-- Starting Event Loop --\n");
  while (!WindowShouldClose()) {
    eventLoop.root();
  }

  // --- CLEAN UP ---
  UnloadShader(shader);
  UnloadFont(fontRetro);
  CloseWindow();

  return 0;
}