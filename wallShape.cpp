#include "raylib.h"
#include "wallShape.h"

void WallShape::DrawWall(Vector2 center, float width, float height, float rotation, Color color) {
  // calc values
  float left = center.x - width / 2;
  float top = center.y - height / 2;
  float right = center.x + width / 2;
  float bottom = center.y + height / 2;

  // draw hitbox
  DrawRectangleV((Vector2){ left, top }, (Vector2){ width, height }, color);
  // draw triangle left
  DrawTriangle((Vector2){left - (float)(0.6 * height), bottom}, (Vector2){left, bottom}, (Vector2){left, top}, color);
  // draw triangle right
  DrawTriangle((Vector2){right, top}, (Vector2){right, bottom}, (Vector2){right + (float)(0.6 * height), bottom}, color);
};