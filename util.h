#include "raylib.h"

class Util {
  public:
    static Vector2 rotate2d(Vector2 center, Vector2 point, float r);
    static Vector2 rotate2d(Vector2 point, float r);
    static float distance(Vector2 a, Vector2 b);
};