#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "util.h"

Vector2 Util::rotate2d(Vector2 c, Vector2 p, float r) {
  float dx = p.x - c.x;
  float dy = p.y - c.y;
  Vector2 o = { c.x + (dx * cos(r) - dy * sin(r)), c.y + (dy * cos(r) + dx * sin(r))};
  return o;
}

Vector2 Util::rotate2d(Vector2 p, float r) {
  Vector2 o = { p.x * cos(r) - p.y * sin(r), p.y * cos(r) + p.x * sin(r)};
  return o;
}

float Util::distance(Vector2 a, Vector2 b) {
  return Vector2Distance(a, b);
}