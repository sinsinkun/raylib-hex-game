#include "raylib.h"
#include "wallShape.h"
#include "math.h"

void WallShape::DrawWall(Vector2 center, float w, float h, float rot, Color color) {
  // calculate polygon point coords
  float w2 = w - 20;
  Vector2 A = { center.x - (w/2 * cos(rot) + h/2 * sin(rot)), center.y + (h/2 * cos(rot) - w/2 * sin(rot)) };
  Vector2 B = { center.x + (w/2 * cos(rot) - h/2 * sin(rot)), center.y + (h/2 * cos(rot) + w/2 * sin(rot)) };
  Vector2 C = { center.x + (w2/2 * cos(rot) + h/2 * sin(rot)), center.y - (h/2 * cos(rot) - w2/2 * sin(rot)) };
  Vector2 D = { center.x - (w2/2 * cos(rot) - h/2 * sin(rot)), center.y - (h/2 * cos(rot) + w2/2 * sin(rot)) };

  // draw triangle left
  DrawTriangle(A, B, C, color);
  // draw triangle right
  DrawTriangle(A, C, D, color);
};