#include "raylib.h"
#include "wall.h"
#include "math.h"
#include "util.h"

void Wall::DrawWall(Vector2 center, float w, float h, float rot, Color color) {
  // calculate polygon point coords
  float w2 = w - 20;
  Vector2 A = { center.x - (w2/2 * cos(rot) + h/2 * sin(rot)), center.y + (h/2 * cos(rot) - w2/2 * sin(rot)) };
  Vector2 B = { center.x + (w2/2 * cos(rot) - h/2 * sin(rot)), center.y + (h/2 * cos(rot) + w2/2 * sin(rot)) };
  Vector2 C = { center.x + (w/2 * cos(rot) + h/2 * sin(rot)), center.y - (h/2 * cos(rot) - w/2 * sin(rot)) };
  Vector2 D = { center.x - (w/2 * cos(rot) - h/2 * sin(rot)), center.y - (h/2 * cos(rot) + w/2 * sin(rot)) };

  // draw triangle left
  DrawTriangle(A, B, C, color);
  // draw triangle right
  DrawTriangle(A, C, D, color);
};

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
  pos.y += d * 30;
  rot += d;
  rotPos = Util::rotate2d(c, pos, rot);
}