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

Vector anglesToVector(double theta, double alpha)
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
    len = sqrt(x*x + y*y + z*z);
    return Vector(x / len, y / len, z / len);
}

int main(void)
{
    const double dt = 1e-13;
    WilsonCloudChamber wilsonCloudChamber;
    Vector borders(1e-4), cubeSize(10);
    wilsonCloudChamber.setBorders(borders);
    wilsonCloudChamber.setDeltaTime(dt);
    wilsonCloudChamber.setElectronSourcBornPeriod(100 * dt);
    // wilsonCloudChamber.setelectronFieldDirection();
    // wilsonCloudChamber.setMagneticFieldDirection();

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 1200;
    InitWindow(screenWidth, screenHeight, "raylib example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 20.0f, 0.0f, 0.0f };   // Camera position
    camera.target = (Vector3){ 5.0f, 5.0f, 5.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector dir = wilsonCloudChamber.getElectronSourceDirection();
    cout << ">>> AAAA <<< " << dir.x() << " " << dir.y() << " " << dir.z() << endl;

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        //camera.position = mul(camera.position, 20 / size(camera.position)); // Disable zoom?
        EM_ASM({
            camera_position_x = $0;
            camera_position_y = $1;
            camera_position_z = $2;
        }, camera.position.x, camera.position.y, camera.position.z);
        vector<Color> colors = {RED, GREEN, BLUE, YELLOW};

        double electron_theta = EM_ASM_DOUBLE({
            return electron_theta;
        });
        double electron_alpha = EM_ASM_DOUBLE({
            return electron_alpha;
        });
        Vector electronDirection = anglesToVector(electron_theta, electron_alpha);
        wilsonCloudChamber.setElectronSourceDirection(electronDirection);
        wilsonCloudChamber.setProtonSourceDirection(electronDirection);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            BeginMode3D(camera);
            
                wilsonCloudChamber.phyCoub();
                auto particleGroupList = wilsonCloudChamber.getUniqParticleGroupList();
                auto particleGroupIt = particleGroupList.begin();
                auto colorIt = colors.begin();
                for (; particleGroupIt != particleGroupList.end(); particleGroupIt++, colorIt++) {
                    auto particleGroup = particleGroupIt->get();
                    for (auto particleIt = particleGroup->begin(); particleIt != particleGroup->end(); particleIt++) {
                        auto particle = particleIt->get();
                        auto coordinate = particle->getCoordinate();
                        DrawSphere({(float) (coordinate.x() / borders.x() * cubeSize.x()), (float) (coordinate.y() / borders.y() * cubeSize.y()), (float) (coordinate.z() / borders.z() * cubeSize.z())}, 0.1, *colorIt);
                    }
                }
                
                // Draw cube
                DrawCubeWires(camera.target, cubeSize.x(), cubeSize.y(), cubeSize.z(), WHITE);

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