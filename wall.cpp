#include "raylib.h"
#include "raymath.h"
#include "wall.h"
#include "math.h"
#include "util.h"
#include <cstdio>

void Wall::draw() {
  // calculate polygon point coords
  vA = { rotPos.x - (w2/2 * cos(rot) + h/2 * sin(rot)), rotPos.y + (h/2 * cos(rot) - w2/2 * sin(rot)) };
  vB = { rotPos.x + (w2/2 * cos(rot) - h/2 * sin(rot)), rotPos.y + (h/2 * cos(rot) + w2/2 * sin(rot)) };
  vC = { rotPos.x + (w/2 * cos(rot) + h/2 * sin(rot)), rotPos.y - (h/2 * cos(rot) - w/2 * sin(rot)) };
  vD = { rotPos.x - (w/2 * cos(rot) - h/2 * sin(rot)), rotPos.y - (h/2 * cos(rot) + w/2 * sin(rot)) };
  // draw polygon
  DrawTriangle(vA, vB, vC, color);
  DrawTriangle(vA, vC, vD, color);
}

void Wall::update(float d, Vector2 c) {
  pos.x = c.x;
  pos.y += d * speed;
  if (rotate) rot += d;
  rotPos = Util::rotate2d(c, pos, rot);
  float cd = Util::distance(rotPos, c);
  w = cd * 1.1547 + 12; // 1.1547 = 2 * tan(PI/6);
  w2 = (cd - h) * 1.1547 + 12;
  // calculate if wall should be removed
  if (cd < 20 || w2 < 20) shouldRemove = true;
}

// check point collision via infinite horizontal raycasting
bool Wall::rayCastCollision(Vector2 p) {
  // set max range of collision space
  if (Util::distance(rotPos, p) > 100) return false;
  int count = 0;
  // edges: vAvB, vBvC, vCvD, vAvD
  Vector2 edges[8] = { vA, vB, vB, vC, vC, vD, vD, vA};
  for (int i=0; i < 8; i+=2) {
    Vector2 p1 = edges[i];
    Vector2 p2 = edges[i+1];
    if (
      (p.y < p1.y) != (p.y < p2.y) &&
      p.x < p1.x + ((p.y - p1.y)/(p2.y - p1.y))*(p2.x - p1.x)
    ) count++;
  }

  // colliding if odd: x%2 == 1
  return count % 2;
}

// check circular collision via SAT
bool Wall::pointRadiusCollision(Vector2 p, float r) {
  // set max range of collision space
  if (Util::distance(rotPos, p) > 100) return false;
  // edges: vAvB, vBvC, vCvD, vAvD
  Vector2 edges[8] = { vA, vB, vB, vC, vC, vD, vD, vA };
  for (int i=0; i < 8; i+=2) {
    Vector2 p1 = edges[i];
    Vector2 p2 = edges[i+1];
    // calculate normal axis
    Vector2 n = Vector2Normalize(Util::normal2d(p1, p2));
    // find min/max of projections
    float min = Vector2DotProduct(vA, n);
    float max = min;
    for (int i=0; i < 8; i+=2) {
      float proj = Vector2DotProduct(edges[i], n);
      if (proj < min) min = proj;
      else if (proj > max) max = proj;
    }
    float projp = Vector2DotProduct(p, n);
    // check maximum + minimum
    if (projp - r > max || projp + r < min) return false;
  }
  // no gaps in axes found == colliding
  return true;
}

// TODO: alternative collision detection: Separating Axis Theorem

void Wall::debug() {
  printf("Wall debug info: ");
  printf("A: (%f, %f) ", vA.x, vA.y);
  printf("B: (%f, %f) ", vB.x, vB.y);
  printf("C: (%f, %f) ", vC.x, vC.y);
  printf("D: (%f, %f) ", vD.x, vD.y);
  printf("rotation: %f\n", rot);
}