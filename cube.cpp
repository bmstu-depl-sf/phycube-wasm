#include "common.h"
#include "WilsonCloudChamber.h"
#include <iostream>
// Some vector funcs

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

using namespace std;
using namespace phycoub;

int main(void)
{
    const double dt = 1e-15;
    WilsonCloudChamber wilsonCloudChamber;
    wilsonCloudChamber.setBorders({2, 2, 2});
    wilsonCloudChamber.setDeltaTime(dt);
    wilsonCloudChamber.setElectronSourcBornPeriod(100 * dt);

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 1200;
    InitWindow(screenWidth, screenHeight, "raylib example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 20.0f, 0.0f, 0.0f };   // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    float angle=0;

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        camera.position = mul(camera.position, 20 / size(camera.position)); // Disable zoom?
        EM_ASM({
            camera_position_x = $0;
            camera_position_y = $1;
            camera_position_z = $2;
        }, camera.position.x, camera.position.y, camera.position.z);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);
            
                wilsonCloudChamber.phyCoub();
                auto particleGroupList = wilsonCloudChamber.getUniqParticleGroupList();
                for (auto particleGroupIt = particleGroupList.begin(); particleGroupIt != particleGroupList.end(); particleGroupIt++) {
                    auto particleGroup = particleGroupIt->get();
                    for (auto particleIt = particleGroup->begin(); particleIt != particleGroup->end(); particleIt++) {
                        auto particle = particleIt->get();
                        auto coordinate = particle->getCoordinate();
                        DrawSphere({(float) coordinate.x() * 100000, (float) coordinate.y() * 100000, (float) coordinate.z() * 100000}, 0.25, RED);
                        cout << coordinate.x() << " " << coordinate.y() << " " << coordinate.z() << endl;
                    }
                }
                
                // Draw cube
                DrawCubeWires((Vector3){0.0f, 0.0f, 0.0f}, 10.0f, 10.0f, 10.0f, WHITE);

                EM_ASM({
                    vector_position_x = $0;
                    vector_position_y = $1;
                    vector_position_z = $2;
                }, 0, -5 + 5 * cosf(angle), 5 * sinf(angle));
                DrawLine3D({0, -5, 0}, {0, -5 + 5 * cosf(angle), 5 * sinf(angle)}, YELLOW);

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