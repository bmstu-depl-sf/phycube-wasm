#include "cube.h"

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
        camera.position.x = EM_ASM_DOUBLE({
            return camera_position_x;
        });
        camera.position.y = EM_ASM_DOUBLE({
            return camera_position_y;
        });
        camera.position.z = EM_ASM_DOUBLE({
            return camera_position_z;
        });

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);

            DrawLine3D({0, -5, 0}, {5, -5, 0}, RED);
            DrawLine3D({0, -5, 0}, {0, 0, 0}, GREEN);
            DrawLine3D({0, -5, 0}, {0, -5, 5}, BLUE);

            double vector_x = EM_ASM_DOUBLE({
                return vector_position_x;
            });
             double vector_y = EM_ASM_DOUBLE({
                return vector_position_y;
            });
             double vector_z = EM_ASM_DOUBLE({
                return vector_position_z;
            });
            DrawLine3D({0, -5, 0}, {static_cast<float> (vector_x), static_cast<float> (vector_y), static_cast<float> (vector_z)}, YELLOW);

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