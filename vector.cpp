#include "common.h"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <emscripten.h>

Vector3 mul(Vector3 a, float b) {
    Vector3 c = {a.x * b, a.y * b, a.z * b};
    return c;
}

float size(Vector3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3 anglesToVector(double theta, double alpha)
{
    theta *= 3.14 / 180;
    alpha *= 3.14 / 180;
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

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 300;
    const int screenHeight = 300;
    InitWindow(screenWidth, screenHeight, "raylib example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 20.0f, 0.0f, 0.0f };   // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 35.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        camera.position.x = EM_ASM_DOUBLE({
            return camera.x;
        });
        camera.position.y = EM_ASM_DOUBLE({
            return camera.y;
        });
        camera.position.z = EM_ASM_DOUBLE({
            return camera.z;
        });
        camera.position = mul(camera.position, 20 / size(camera.position)); 

        double theta = EM_ASM_DOUBLE({
            return directionById($0).theta;
        }, OBJECT);
        double alpha = EM_ASM_DOUBLE({
            return directionById($0).alpha;
        }, OBJECT);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);

            DrawLine3D({5, 0, 0}, {0, 0, 0}, RED);
            DrawCylinderEx({5, 0, 0}, {5.5, 0, 0}, 0.15, 0, 25, RED);
            DrawLine3D({0, 5, 0}, {0, 0, 0}, GREEN);
            DrawCylinderEx({0, 5, 0}, {0, 5.5, 0}, 0.15, 0, 25, GREEN);
            DrawLine3D({0, 0, 5}, {0, 0, 0}, BLUE);
            DrawCylinderEx({0, 0, 5}, {0, 0, 5.5}, 0.15, 0, 25, BLUE);

            DrawLine3D(anglesToVector(theta, alpha), {0, 0, 0}, YELLOW);
            DrawCylinderEx(anglesToVector(theta, alpha), mul(anglesToVector(theta, alpha), 5.5/5.0), 0.15, 0, 25, YELLOW);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}