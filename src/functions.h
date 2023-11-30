#include <raylib.h>
#include <Vector.h>
#include <cmath>

#define TITLE "Камера Вильсона"
#define FPS 60

using namespace phycoub;

Vector3 sum(Vector3 a, Vector3 b);
Vector3 mul(Vector3 a, float b);
float size(Vector3 a);
Vector anglesToVector(double theta, double alpha);
Vector3 anglesToVector3(double theta, double alpha);