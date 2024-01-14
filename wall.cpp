#include "raylib.h"
#include "wall.h"
#include "math.h"
#include "util.h"

void Wall::draw() {
  // calculate polygon point coords
  Vector2 A = { rotPos.x - (w2/2 * cos(rot) + h/2 * sin(rot)), rotPos.y + (h/2 * cos(rot) - w2/2 * sin(rot)) };
  Vector2 B = { rotPos.x + (w2/2 * cos(rot) - h/2 * sin(rot)), rotPos.y + (h/2 * cos(rot) + w2/2 * sin(rot)) };
  Vector2 C = { rotPos.x + (w/2 * cos(rot) + h/2 * sin(rot)), rotPos.y - (h/2 * cos(rot) - w/2 * sin(rot)) };
  Vector2 D = { rotPos.x - (w/2 * cos(rot) - h/2 * sin(rot)), rotPos.y - (h/2 * cos(rot) + w/2 * sin(rot)) };
  // draw polygon
  DrawTriangle(A, B, C, color);
  DrawTriangle(A, C, D, color);
}

void Wall::update(float d, Vector2 c) {
  pos.y += d * 50;
  rot += d;
  rotPos = Util::rotate2d(c, pos, rot);
  float cd = Util::distance(rotPos, c);
  w = cd * 1.1547; // 1.1547 = 2 * tan(PI/6);
  w2 = (cd - h) * 1.1547;
  // calculate if wall should be removed
  if (cd < 20 || w2 < 20) shouldRemove = true;
}