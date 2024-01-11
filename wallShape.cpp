#include "raylib.h"
#include "wallShape.h"

void WallShape::DrawWall(Vector2 center, float width, float height, float rotation, Color color) {
  // calc values
  float left = center.x - width / 2;
  float top = center.y - height / 2;
  float right = center.x + width / 2;
  float bottom = center.y + height / 2;

  // draw hitbox
  DrawRectangle((int)left, (int)top, (int)width, (int)height, color);
  // draw triangle left
  DrawTriangle((Vector2){left - 30, bottom}, (Vector2){left, bottom}, (Vector2){left, top}, color);
  // draw triangle right
  DrawTriangle((Vector2){right, top}, (Vector2){right, bottom}, (Vector2){right + 30, bottom}, color);
};