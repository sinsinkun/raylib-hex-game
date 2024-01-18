#include "raylib.h"
#include "raymath.h"
#include "wall.h"
#include "math.h"
#include "util.h"
#include <cstdio>

void Wall::spawn(WallType type, Vector2 pos, int rotPos, float lifetime) {
  this->type = type;
  this->spawned = true;
  this->rotate = true;
  this->pos = pos;
  this->rot = lifetime + (rotPos * PI / 3);
  this->speed = 120.0 + lifetime * 2.0;

  if (type == WallType::TriCollide) {
    this->color = BLUE;
  } else if (type == WallType::MouseCollide) {
    this->color = GREEN;
  }
};

void Wall::reset() {
  type = WallType::Unspawned;
  spawned = false;
  shouldRemove = false;
  pos = Vector2 { 0.0, 0.0 };
  rot = 0.0;
  color = BLACK;
  rotPos = Vector2 { 999999.0, 999999.0 };
}

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
  w = cd * 1.1547 + (3*h / 5); // 1.1547 = 2 * tan(PI/6);
  w2 = (cd - h) * 1.1547 + (3*h / 5);
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
  // edges: vAvB, vBvC, vCvD (ignore vDvA)
  Vector2 edges[6] = { vA, vB, vB, vC, vC, vD };
  for (int i=0; i < 6; i+=2) {
    Vector2 p1 = edges[i];
    Vector2 p2 = edges[i+1];
    // calculate normal axis
    Vector2 n = Vector2Normalize(Util::normal2d(p1, p2));
    // find min/max of projections
    float min = Vector2DotProduct(vA, n);
    float max = min;
    for (int i=0; i < 6; i+=2) {
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

void Wall::debug() {
  printf("Wall debug info: ");
  printf("A: (%f, %f) ", vA.x, vA.y);
  printf("B: (%f, %f) ", vB.x, vB.y);
  printf("C: (%f, %f) ", vC.x, vC.y);
  printf("D: (%f, %f) ", vD.x, vD.y);
  printf("rotation: %f\n", rot);
}

// --- PATTERNS ---

WallSpawnData WallPatterns::staircase[30] = {
  WallSpawnData { TriCollide, -500.0, 0 },
  WallSpawnData { TriCollide, -520.0, 1 },
  WallSpawnData { TriCollide, -540.0, 2 },
  WallSpawnData { TriCollide, -560.0, 3 },
  WallSpawnData { TriCollide, -580.0, 4 },
  WallSpawnData { TriCollide, -600.0, 5 },
  WallSpawnData { TriCollide, -620.0, 0 },
  WallSpawnData { TriCollide, -640.0, 1 },
  WallSpawnData { TriCollide, -660.0, 2 },
  WallSpawnData { TriCollide, -680.0, 3 },
  WallSpawnData { TriCollide, -700.0, 4 },
  WallSpawnData { TriCollide, -720.0, 5 },
  WallSpawnData { TriCollide, -740.0, 0 },
  WallSpawnData { TriCollide, -760.0, 1 },
  WallSpawnData { TriCollide, -780.0, 2 },
  WallSpawnData { TriCollide, -800.0, 3 },
  WallSpawnData { TriCollide, -820.0, 4 },
  WallSpawnData { TriCollide, -840.0, 5 },
  WallSpawnData { TriCollide, -860.0, 0 },
  WallSpawnData { TriCollide, -880.0, 1 },
  WallSpawnData { TriCollide, -900.0, 2 },
  WallSpawnData { TriCollide, -920.0, 3 },
  WallSpawnData { TriCollide, -940.0, 4 },
  WallSpawnData { TriCollide, -960.0, 5 },
  WallSpawnData { TriCollide, -980.0, 0 },
  WallSpawnData { TriCollide, -1000.0, 1 },
  WallSpawnData { TriCollide, -1020.0, 2 },
  WallSpawnData { TriCollide, -1040.0, 3 },
  WallSpawnData { TriCollide, -1060.0, 4 },
  WallSpawnData { TriCollide, -1080.0, 5 },
};
WallSpawnData WallPatterns::alternate1[30] = {
  WallSpawnData { TriCollide, -500.0, 0 },
  WallSpawnData { TriCollide, -500.0, 2 },
  WallSpawnData { TriCollide, -500.0, 4 },
  WallSpawnData { TriCollide, -600.0, 1 },
  WallSpawnData { TriCollide, -600.0, 3 },
  WallSpawnData { TriCollide, -600.0, 5 },
  WallSpawnData { MouseCollide, -600.0, 0 },
  WallSpawnData { MouseCollide, -600.0, 2 },
  WallSpawnData { MouseCollide, -600.0, 4 },
  WallSpawnData { TriCollide, -700.0, 0 },
  WallSpawnData { TriCollide, -700.0, 2 },
  WallSpawnData { TriCollide, -700.0, 4 },
  WallSpawnData { TriCollide, -800.0, 1 },
  WallSpawnData { TriCollide, -800.0, 3 },
  WallSpawnData { TriCollide, -800.0, 5 },
  WallSpawnData { TriCollide, -900.0, 0 },
  WallSpawnData { TriCollide, -900.0, 2 },
  WallSpawnData { TriCollide, -900.0, 4 },
  WallSpawnData { TriCollide, -1000.0, 1 },
  WallSpawnData { TriCollide, -1000.0, 3 },
  WallSpawnData { TriCollide, -1000.0, 5 },
  WallSpawnData { MouseCollide, -1000.0, 0 },
  WallSpawnData { MouseCollide, -1000.0, 2 },
  WallSpawnData { MouseCollide, -1000.0, 4 },
  WallSpawnData { TriCollide, -1100.0, 0 },
  WallSpawnData { TriCollide, -1100.0, 2 },
  WallSpawnData { TriCollide, -1100.0, 4 },
  WallSpawnData { TriCollide, -1200.0, 1 },
  WallSpawnData { TriCollide, -1200.0, 3 },
  WallSpawnData { TriCollide, -1200.0, 5 },
};
WallSpawnData WallPatterns::alternate2[24] = {
  WallSpawnData { TriCollide,   -500.0, 0 },
  WallSpawnData { TriCollide,   -500.0, 2 },
  WallSpawnData { TriCollide,   -500.0, 4 },
  WallSpawnData { MouseCollide, -600.0, 1 },
  WallSpawnData { TriCollide,   -600.0, 3 },
  WallSpawnData { MouseCollide, -600.0, 5 },
  WallSpawnData { TriCollide,   -800.0, 0 },
  WallSpawnData { TriCollide,   -800.0, 2 },
  WallSpawnData { TriCollide,   -800.0, 4 },
  WallSpawnData { MouseCollide, -900.0, 1 },
  WallSpawnData { TriCollide,   -900.0, 3 },
  WallSpawnData { MouseCollide, -900.0, 5 },
  WallSpawnData { TriCollide,   -1100.0, 0 },
  WallSpawnData { TriCollide,   -1100.0, 2 },
  WallSpawnData { TriCollide,   -1100.0, 4 },
  WallSpawnData { MouseCollide, -1200.0, 1 },
  WallSpawnData { TriCollide,   -1200.0, 3 },
  WallSpawnData { MouseCollide, -1200.0, 5 },
  WallSpawnData { TriCollide,   -1300.0, 0 },
  WallSpawnData { TriCollide,   -1300.0, 2 },
  WallSpawnData { TriCollide,   -1300.0, 4 },
  WallSpawnData { MouseCollide, -1400.0, 1 },
  WallSpawnData { TriCollide,   -1400.0, 3 },
  WallSpawnData { MouseCollide, -1400.0, 5 },
};
