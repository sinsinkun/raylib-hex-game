#include "raylib.h"

class Wall {
  public:
    bool spawned = false;
    bool shouldRemove = false;
    bool rotate = false;
    Vector2 pos = { 0, 0 };
    float speed = 50;
    float w;
    float w2 = (w - h) * 1.1547; // 1.1547 = 2 * tan(PI/6)
    float h;
    float rot;
    Color color;
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