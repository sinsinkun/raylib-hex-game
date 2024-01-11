#include "raylib.h"
#include "wallShape.h"

void WallShape::DrawWall(Vector2 center, float width, float height, float rotation, Color color) {
  float left = center.x - width / 2;
  float top = center.y - height / 2;
  DrawRectangle((int)left, (int)top, (int)width, (int)height, color);
};