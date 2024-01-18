#include <cstdio>
#include "math.h"
#include "raylib.h"
#include "raymath.h"
#include "util.h"
#include "shapes.h"

using namespace Shapes;

/* -- HEXAGON METHODS -- */

void Hexagon::draw() {
  // need to convert angle from rad to deg
  DrawPoly(pos, 6, 50, angle * 180.0 / PI, color);
}

void Hexagon::update(float d, Vector2 c) {
  pos = c;
  if (rotate) angle += d;
}

/* -- TRIANGLE METHODS -- */

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

/* -- CURSOR METHODS -- */

void Cursor::update(Vector2 p) {
  pos = p;
}

void Cursor::draw() {
  DrawCircle((int)pos.x, (int)pos.y, radius, color);
}