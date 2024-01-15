#include "raylib.h"
#include "raymath.h"
#include "triangle.h"

void Triangle::update(Vector2 c, Vector2 m) {
  Vector2 relPos = Vector2Subtract(m, c);
  Vector2 normalizedPos = Vector2Normalize(relPos);
  normalizedPos.x = centerOffset * normalizedPos.x;
  normalizedPos.y = centerOffset * normalizedPos.y;
  pos = Vector2Add(c, normalizedPos);
  angle = atan2(relPos.y, relPos.x);
}

void Triangle::draw() {
  DrawPoly(pos, 3, 10, angle * 180.0 / PI, color);
}