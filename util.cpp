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

Vector2 Util::projection2d(Vector2 a, Vector2 b) {
  float k = Vector2DotProduct(a, b) / Vector2DotProduct(b, b);
  Vector2 o = { k * b.x, k * b.y };
  return o;
}

float Util::distance(Vector2 a, Vector2 b) {
  return Vector2Distance(a, b);
}

Vector2 Util::normal2d(Vector2 a, Vector2 b) {
  float dx = b.x - a.x;
  float dy = b.y - a.y;
  return Vector2 { -dy, dx };
}