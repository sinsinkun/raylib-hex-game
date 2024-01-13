#include "raylib.h"

class Wall {
  public:
    bool spawned;
    Vector2 pos;
    float w;
    float w2 = w - 20;
    float h;
    float rot;
    Color color;
    void update(float delta, Vector2 center);
    void draw();
    // bool shouldRemove(); TODO
    static void DrawWall(Vector2 center, float width, float height, float rotation, Color color);
  private:
    Vector2 rotPos;
};