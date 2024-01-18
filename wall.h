#include "raylib.h"

enum WallType {
  Unspawned,
  TriCollide,
  MouseCollide,
};

class Wall {
  public:
    WallType type = Unspawned;
    bool spawned = false;
    bool shouldRemove = false;
    bool rotate = false;
    Vector2 pos = { 0.0, 0.0 };
    float speed = 100.0;
    float w = 100.0;
    float w2 = 92.4;
    float h = 30.0;
    float rot = 0.0;
    Color color = BLACK;
    void spawn(WallType type, Vector2 pos, int rotPos, float lifetime);
    void reset();
    void update(float delta, Vector2 center);
    void draw();
    bool rayCastCollision(Vector2 point);
    bool pointRadiusCollision(Vector2 point, float radius);
    void debug();
  private:
    Vector2 rotPos = { 999999.0, 999999.0 };
    Vector2 vA = { 999999.0, 999999.0 };
    Vector2 vB = { 999999.0, 999999.0 };
    Vector2 vC = { 999999.0, 999999.0 };
    Vector2 vD = { 999999.0, 999999.0 };
};

// helper class for creating wall patterns
class WallSpawnData {
  public:
    WallType type = Unspawned;
    float offset = -100.0;
    int rotPos = 0;
};

class WallPatterns {
  public:
    static WallSpawnData staircase[30];
    static WallSpawnData alternate1[30];
    static WallSpawnData alternate2[24];
};