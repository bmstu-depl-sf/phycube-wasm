#include "raylib.h"
#include <cmath>
#include <vector>
#include <emscripten.h>

// Simple point class
class Point {
private:
    Vector3 speed, boost; 
public:
    float radius;
    Color color;
    Vector3 position;

    Point(Color color, float radius = 1.0f, Vector3 position = {0, 0, 0}, Vector3 speed = {0, 0, 0}, Vector3 boost = {0, 0, 0}):
        speed(speed), boost(boost), radius(radius), color(color), position(position) {};

    void move(); 
};

float size(Vector3 a);
Vector3 mul(Vector3 a, float b);
Vector3 sum(Vector3 a, Vector3 b);