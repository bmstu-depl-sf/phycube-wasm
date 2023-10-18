#include "raylib.h"

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
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);

                // Add axes & vector here (static or randomly moving)
                DrawSphere({-5, 0, 0}, 1.0f, RED);
                DrawSphere({0, 0, 0}, 1.0f, GREEN);
                DrawSphere({5, 0, 0}, 1.0f, BLUE);

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