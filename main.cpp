#include "raylib.h"
#include <cmath>

// Example Entity class
//--------------------------------------------------------------------------------------
class RotatingSphere {
private:
    float angle, traectoryRadius, traectorySpeed;
public:
    float radius;
    Color color;
    Vector3 position;

    RotatingSphere(Color color, float radius = 1.0f, float angle = 0, float traectoryRadius = 0, float traectorySpeed = 0):
        angle(angle), traectoryRadius(traectoryRadius), traectorySpeed(traectorySpeed), radius(radius), color(color), position() {};
    
    void move() {
        angle += traectorySpeed;
        position.x = sinf(angle) * traectoryRadius;
        position.y = cosf(angle) * traectoryRadius;
    }
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib example");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    RotatingSphere spheres[] = {
        RotatingSphere(RED, 0.25f, 0.0f, 1.5f, 0.1f),
        RotatingSphere(GREEN, 0.5f, 1.0f, 1.5f, 0.1f),
        RotatingSphere(BLUE, 0.75f, 2.0f, 1.5f, 0.1f)
    };

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            
            for (auto &sphere: spheres) {
                sphere.move();
                DrawSphere(sphere.position, sphere.radius, sphere.color);
            }

            DrawGrid(10, 1.0f);

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