#include "raylib.h"

class Util {
  public:
    static Vector2 rotate2d(Vector2 center, Vector2 point, float r);
    static Vector2 rotate2d(Vector2 point, float r);
    static Vector2 projection2d(Vector2 a, Vector2 b);
    static float distance(Vector2 a, Vector2 b);
    static Vector2 normal2d(Vector2 a, Vector2 b);
};