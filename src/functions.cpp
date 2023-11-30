#include "functions.h"

Vector3 sum(Vector3 a, Vector3 b) {
    Vector3 c = {a.x + b.x, a.y + b.y, a.z + b.z};
    return c;
}

Vector3 mul(Vector3 a, float b) {
    Vector3 c = {a.x * b, a.y * b, a.z * b};
    return c;
}

float size(Vector3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector anglesToVector(double theta, double alpha)
{
    theta *= M_PI / 180;
    alpha *= M_PI / 180;
    double x, y, z, len;
    if (alpha) {
        x = sin(theta);
        y = cos(theta);
        z = 1.0 / tan(alpha);
    }
    else {
        x = 0;
        y = 0;
        z = 1;
    }
    len = sqrt(x*x + y*y + z*z);
    return Vector(x / len, y / len, z / len);
}

Vector3 anglesToVector3(double theta, double alpha)
{
    theta *= M_PI / 180;
    alpha *= M_PI / 180;
    double x, y, z, len;
    if (alpha) {
        x = sin(theta);
        y = cos(theta);
        z = 1.0 / tan(alpha);
    }
    else {
        x = 0;
        y = 0;
        z = 1;
    }
    len = sqrt(x*x + y*y + z*z) / 5;
    Vector3 result = {static_cast<float>(x / len), static_cast<float>(y / len), static_cast<float>(z / len)};
    return result;
}
