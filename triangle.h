#include "raylib.h"

class Triangle {
  public:
    Vector2 pos = {0, 0};
    float angle = 0.0;
    float centerOffset = 65.0;
    float radius = 5.0;
    Color color;
    void update(Vector2 center, Vector2 mousePos);
    void draw();
};