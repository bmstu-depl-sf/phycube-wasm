#include "common.h"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <emscripten.h>

Vector3 mul(Vector3 a, float b) {
    Vector3 c = {a.x * b, a.y * b, a.z * b};
    return c;
}

Vector3 anglesToVector(double theta, double alpha)
{
    theta *= 3.14 / 180;
    alpha *= 3.14 / 180;
    printf("%.2f %.2f\n", theta, alpha);
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
        UpdateCamera(&camera, CAMERA_ORBITAL);
        camera.position.x = EM_ASM_DOUBLE({
            return camera_position_x;
        });
        camera.position.y = EM_ASM_DOUBLE({
            return camera_position_y;
        });
        camera.position.z = EM_ASM_DOUBLE({
            return camera_position_z;
        });

        double electron_theta = EM_ASM_DOUBLE({
            return electron_theta;
        });
        double electron_alpha = EM_ASM_DOUBLE({
            return electron_alpha;
        });

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);

            // DrawLine3D({0, -5, 0}, {5, -5, 0}, RED);
            // DrawLine3D({0, -5, 0}, {0, 0, 0}, GREEN);
            // DrawLine3D({0, -5, 0}, {0, -5, 5}, BLUE);

            DrawLine3D({5, 0, 0}, {0, 0, 0}, RED);
            DrawLine3D({0, 5, 0}, {0, 0, 0}, GREEN);
            DrawLine3D({0, 0, 5}, {0, 0, 0}, BLUE);

            // DrawLine3D({0, -5, 0}, {static_cast<float> (vector_x), static_cast<float> (vector_y), static_cast<float> (vector_z)}, YELLOW);
            
             DrawLine3D(anglesToVector(electron_theta, electron_alpha), {0, 0, 0}, YELLOW);

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