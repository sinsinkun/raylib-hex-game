#include "raylib.h"

class Wall {
  public:
    bool spawned;
    bool shouldRemove = false;
    bool rotate = false;
    Vector2 pos;
    float speed = 50;
    float w;
    float w2 = (w - h) * 1.1547; // 1.1547 = 2 * tan(PI/6)
    float h;
    float rot;
    Color color;
    void update(float delta, Vector2 center);
    void draw();
  private:
    Vector2 rotPos;
};