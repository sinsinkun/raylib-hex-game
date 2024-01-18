#include <cstdio> // imports printf
#include "raylib.h"
#include "raymath.h"
#include "util.h"
#include "shapes.h"
#include "wall.h"

#define GLSL_VERSION 330

const int WALL_NUM = 200;

enum Scene {
  menu,
  game
};

enum PauseType {
  unpaused,
  paused,
  gameover
};

class EventLoop {
  public:
    PauseType paused = unpaused;
    Shader shaders[2];
    Font font;
    Color primaryColor = WHITE;
    Scene scene;
    float lifetime = 0;
    Shapes::Hexagon hex;
    Shapes::Triangle tri;
    Wall walls[WALL_NUM];
    WallSpawnData* wallPatterns[3];
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
    if (IsWindowFocused()) {
      // draw background
      BeginShaderMode(shaders[0]);
        DrawRectangle(0, 0, w, h, WHITE);
      EndShaderMode();
      // draw text overlay
      DrawTextEx(font, TextFormat("FPS: %i", fps), (Vector2){ 10, 10 }, 20, 3.5, WHITE);
      DrawTextEx(font, TextFormat("Duration: %.*f", 2, lifetime), (Vector2){ 10, (float)h - 30 }, 20, 3.5, WHITE);
      DrawTextEx(font, "Hit space to enter game", (Vector2){ (float)w/2 - 170, (float)h/2 - 10 }, 20, 3.5, WHITE);
    } else {
      DrawText("Pay Attention to me", 10, 10, 20, RED);
    }
  EndDrawing();

  // --- REGISTER INPUT ---
  if (IsKeyPressed(KEY_SPACE)) {
    scene = Scene::game;
    lifetime = 0;
    hex.angle = 0;
    paused = PauseType::unpaused;
  }
}

void EventLoop::game() {
  // --- UPDATE STATE ---
  // get world parameters
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  int fps = GetFPS();
  float deltaT = GetFrameTime();
  Vector2 mousePos = GetMousePosition();
  Vector2 screenCenter = { (float)w/2, (float)h/2 };

  // hide cursor
  if (!IsCursorHidden()) {
    HideCursor();
  }
  
  if (paused == PauseType::unpaused) {
    lifetime += deltaT;
    // calculate triangle position and rotation
    tri.update(screenCenter, mousePos);
    // calculate hex position and rotation
    hex.update(deltaT, screenCenter);
  }

  // update walls
  if (paused == PauseType::unpaused) {
    // update existing wall cache
    float minDistance = 0.0;
    for (int i=0; i<WALL_NUM; i++) {
      if (minDistance > walls[i].pos.y) {
        minDistance = walls[i].pos.y;
      }
      if (!walls[i].spawned) continue;
      if (walls[i].shouldRemove) {
        walls[i].reset();
      } else {
        walls[i].update(deltaT, screenCenter);
        if (walls[i].type == WallType::TriCollide && walls[i].pointRadiusCollision(tri.pos, 6.0)) {
          if (Util::distance(screenCenter, walls[i].pos) < 45.0) {
            // if wall is behind triangle, ignore collision
            continue;
          }
          walls[i].color = RED;
          printf("Collided with wall (tri) %i\n", i);
          paused = PauseType::gameover;
        } else if (walls[i].type == WallType::MouseCollide && walls[i].pointRadiusCollision(mousePos, 15.0)) {
          walls[i].color = MAGENTA;
          printf("Collided with wall (mouse) %i\n", i);
          paused = PauseType::gameover;
        };
      }
    }
    // spawn new wall pattern when furthest wall is 400px from view edge
    if (minDistance > -400.0) {
      // pick pattern to spawn
      int randNum = GetRandomValue(0, 2);
      int size = sizeof(wallPatterns[randNum]);
      WallSpawnData pattern[size];
      // dereference pointer
      for (int i=0; i<size; i++) {
        pattern[i] = wallPatterns[randNum][i];
      }
      int j = 0;
      printf("DEBUG: Spawning new pattern %i\n", randNum);
      for (int i=0; i<WALL_NUM; i++) {
        // exit early if pattern is done
        if (j == size) break;
        if (!walls[i].spawned) {
          walls[i].spawn(
            pattern[j].type, 
            Vector2{ screenCenter.x, pattern[j].offset }, 
            pattern[j].rotPos, 
            lifetime
          );
          j++;
        }
      }
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
    // draw background
    BeginShaderMode(shaders[0]);
      DrawRectangle(0, 0, w, h, WHITE);
    EndShaderMode();

    // draw walls
    for (int i=0; i<WALL_NUM; i++) {
      if (!walls[i].spawned) continue;
      walls[i].draw();
    }

    // draw hex
    hex.draw();
    // draw user triangle
    tri.draw();

    // draw pointer
    BeginShaderMode(shaders[1]);
      DrawCircle((int)mousePos.x, (int)mousePos.y, 20, BLACK);
    EndShaderMode();

    // draw text overlay
    DrawTextEx(font, TextFormat("FPS: %i", fps), (Vector2){ 10, 10 }, 20, 3.5, WHITE);
    DrawTextEx(font, TextFormat("Duration: %.*f", 2, lifetime), (Vector2){10, (float)h - 30}, 20, 3.5, WHITE);
    if (paused == PauseType::paused) {
      DrawTextEx(font, "Paused", (Vector2){ (float)w/2 - 70, (float)h/2 - 12 }, 30, 3.5, WHITE);
    }
    if (paused == PauseType::gameover) {
      DrawTextEx(font, TextFormat("Game Over! You survived %.*f Seconds.", 2, lifetime), (Vector2){ (float)w/2 - 250, (float)h/2}, 20, 3.5, WHITE);
      DrawTextEx(font, "Hit space to go back to menu", (Vector2){ (float)w/2 - 200, (float)h/2 + 30}, 20, 3.5, WHITE);
    }
  EndDrawing();

  // --- REGISTER INPUT ---
  if (IsKeyPressed(KEY_SPACE)) {
    if (paused == PauseType::unpaused) {
      paused = PauseType::paused;
      return;
    }
    if (paused == PauseType::paused) {
      paused = PauseType::unpaused;
      return;
    }
    scene = Scene::menu;
    // reset walls
    for (int i=0; i<WALL_NUM; i++) {
      walls[i].reset();
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
  eventLoop.tri.color = primary;
  eventLoop.hex.color = primary;
  eventLoop.hex.rotate = true;

  // add wall patterns
  eventLoop.wallPatterns[0] = WallPatterns::staircase;
  eventLoop.wallPatterns[1] = WallPatterns::alternate1;
  eventLoop.wallPatterns[2] = WallPatterns::alternate2;
  
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