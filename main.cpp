#include <cstdio> // imports printf
#include "raylib.h"
#include "raymath.h"
#include "wall.h"
#include "util.h"
#include "timer.h"

#define GLSL_VERSION 330

enum Scene {
  menu,
  game
};

class EventLoop {
  public:
    Shader shaders[2];
    Font font;
    Color primaryColor = WHITE;
    Scene scene;
    float lifetime = 0;
    Wall walls[50];
    BurstTimer spawnTimer;
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

  // show cursor
  if (IsCursorHidden()) {
    ShowCursor();
  }

  // add uniforms to background shader
  float screenSize[2] = { (float)w, (float)h };
  float t = (float)elapsed;
  SetShaderValue(shaders[0], GetShaderLocation(shaders[0], "t"), &t, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders[0], GetShaderLocation(shaders[0], "screen_size"), &screenSize, SHADER_UNIFORM_VEC2);

  // --- DRAW TO SCREEN ---
  BeginDrawing();
    ClearBackground(BLACK);
    // draw background
    BeginShaderMode(shaders[0]);
      DrawRectangle(0, 0, w, h, WHITE);
    EndShaderMode();

    // draw text overlay
    DrawTextEx(font, TextFormat("FPS: %i", fps), (Vector2){ 10, 10 }, 20, 3.5, WHITE);
    DrawTextEx(font, TextFormat("Duration: %.*f", 2, lifetime), (Vector2){ 10, (float)h - 30 }, 20, 3.5, WHITE);
    DrawTextEx(font, "Hit space to enter game", (Vector2){ (float)w/2 - 170, (float)h/2 - 10 }, 20, 3.5, WHITE);
  EndDrawing();

  // --- REGISTER INPUT ---
  if (IsKeyPressed(KEY_SPACE)) {
    scene = Scene::game;
    lifetime = 0;
  }
}

void EventLoop::game() {
  // --- UPDATE STATE ---
  // get world parameters
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  int fps = GetFPS();
  lifetime += GetFrameTime();
  Vector2 mousePos = GetMousePosition();
  Vector2 screenCenter = { (float)w/2, (float)h/2 };

  // hide cursor
  if (!IsCursorHidden()) {
    HideCursor();
  }
  // calculate triangle position and rotation
  Vector2 relPos = Vector2Subtract(mousePos, screenCenter);
  Vector2 normalizedPos = Vector2Normalize(relPos);
  normalizedPos.x = 65.0 * normalizedPos.x;
  normalizedPos.y = 65.0 * normalizedPos.y;
  Vector2 absPos = Vector2Add(screenCenter, normalizedPos);
  float relAngle = atan2(relPos.y, relPos.x);

  // spawn wall based on timer
  spawnTimer.update(GetFrameTime());
  // spawn new wall
  for (int i=0; i<50; i++) {
    // spawn new wall
    if (spawnTimer.tick() && !walls[i].spawned) {
      walls[i].spawned = true;
      walls[i].w = screenCenter.y * 1.1547;
      walls[i].w2 = (screenCenter.y - 20) * 1.1547;
      walls[i].h = 20;
      walls[i].color = RED;
      walls[i].rot = 0;
      walls[i].pos = Vector2{ screenCenter.x, -20 };
      break;
    }
  }

  // update walls
  for (int i=0; i<50; i++) {
    if (!walls[i].spawned) continue;
    if (walls[i].shouldRemove) {
      walls[i].spawned = false;
      walls[i].shouldRemove = false;
    } else {
      walls[i].update(GetFrameTime(), screenCenter);
    }
  }

  // add uniforms to background shader
  float screenSize[2] = { (float)w, (float)h };
  SetShaderValue(shaders[0], GetShaderLocation(shaders[0], "t"), &lifetime, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders[0], GetShaderLocation(shaders[0], "screen_size"), &screenSize, SHADER_UNIFORM_VEC2);
  SetShaderValue(shaders[1], GetShaderLocation(shaders[1], "t"), &lifetime, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders[1], GetShaderLocation(shaders[1], "screen_size"), &screenSize, SHADER_UNIFORM_VEC2);
  SetShaderValue(shaders[1], GetShaderLocation(shaders[1], "mouse_pos"), &mousePos, SHADER_UNIFORM_VEC2);

  // --- DRAW TO SCREEN ---
  BeginDrawing();
    ClearBackground(BLACK);
    if (IsWindowFocused()) {
      // draw background
      BeginShaderMode(shaders[0]);
        DrawRectangle(0, 0, w, h, WHITE);
      EndShaderMode();

      // draw walls
      for (int i=0; i<50; i++) {
        if (!walls[i].spawned) continue;
        walls[i].draw();
      }
      
      // draw rotating hex
      DrawPoly(screenCenter, 6, 50, lifetime * 180.0 / PI, primaryColor);
      // draw user triangle
      DrawPoly(absPos, 3, 10, relAngle * 180.0 / PI, primaryColor);

      // draw pointer
      BeginShaderMode(shaders[1]);
        DrawCircle((int)mousePos.x, (int)mousePos.y, 20, BLACK);
      EndShaderMode();

      // draw text overlay
      DrawTextEx(font, TextFormat("FPS: %i", fps), (Vector2){ 10, 10 }, 20, 3.5, WHITE);
      DrawTextEx(font, TextFormat("Duration: %.*f", 2, lifetime), (Vector2){ 10, (float)h - 30 }, 20, 3.5, WHITE);

    } else {
      DrawText("Pay attention to me", 10, 10, 24, RED);
    }
  EndDrawing();

  // --- REGISTER INPUT ---
  if (IsKeyPressed(KEY_SPACE)) {
    scene = Scene::menu;
    // reset walls
    for (int i=0; i<50; i++) {
      walls[i].spawned = false;
      walls[i].shouldRemove = false;
    }
  }
}

int main() {
  // --- INITIALIZATION ---
  // SetWindowIcon(image);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Window configuration flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(800, 600, "Hex 2");
  SetWindowMinSize(400, 300);
  SetTargetFPS(60);

  // TTF font : Font data and atlas are generated directly from TTF
  Font fontRetro = LoadFontEx("assets/retro_computer.ttf", 32, 0, 0);
  SetTextLineSpacing(48);

  Shader bgShader = LoadShader(0, TextFormat("assets/bg.frag", GLSL_VERSION));
  Shader cursorShader = LoadShader(0, TextFormat("assets/cursor.frag", GLSL_VERSION));
  Color primary = GetColor(0xafaaecff);

  // add assets to event loop
  EventLoop eventLoop;
  eventLoop.shaders[0] = bgShader;
  eventLoop.shaders[1] = cursorShader;
  eventLoop.font = fontRetro;
  eventLoop.primaryColor = primary;
  eventLoop.scene = Scene::menu;
  eventLoop.spawnTimer.duration = 2.0;
  eventLoop.spawnTimer.repeat = true;
  
  // --- EVENT LOOP ---
  printf("\n\n\n-- Starting Event Loop --\n");
  while (!WindowShouldClose()) {
    eventLoop.root();
  }

  // --- CLEAN UP ---
  for (Shader s : eventLoop.shaders) {
    UnloadShader(s);
  }
  UnloadFont(fontRetro);
  CloseWindow();

  return 0;
}