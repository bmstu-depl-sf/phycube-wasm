#include "common.h"
#include "WilsonCloudChamber.h"
#include <map>
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

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    map<Particle*, vector<Vector>> traectories;
    typedef pair<Particle*, vector<Vector>> particle_pair;
    uint64_t ticks = 0;

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        ticks++;
        vector<Particle*> used(0), unused(0);
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        camera.position = sum(mul(sum(camera.position, {-5, -5, -5}), 20 / size(sum(camera.position, {-5, -5, -5}))), {5, 5, 5}); // Disable zoom?
        EM_ASM({
            camera.x = $0;
            camera.y = $1;
            camera.z = $2;
        }, camera.position.x, camera.position.y, camera.position.z);
        vector<Color> colors = {RED, GREEN, BLUE, YELLOW};

        double shoot_theta = EM_ASM_DOUBLE({
            return directions.shoot.theta;
        });
        double shoot_alpha = EM_ASM_DOUBLE({
            return directions.shoot.alpha;
        });
        Vector shootDirection = anglesToVector(shoot_theta, shoot_alpha);
        wilsonCloudChamber.setElectronSourceDirection(shootDirection);
        wilsonCloudChamber.setProtonSourceDirection(shootDirection);

        double magnetic_theta = EM_ASM_DOUBLE({
            return directions.magnetic.theta;
        });
        double magnetic_alpha = EM_ASM_DOUBLE({
            return directions.magnetic.alpha;
        });
        double magnetic_force = EM_ASM_DOUBLE({
            return directions.magnetic.force;
        });
        wilsonCloudChamber.setMagneticFieldDirection(anglesToVector(magnetic_theta, magnetic_alpha));
        wilsonCloudChamber.setMagneticFieldInduction(magnetic_force);

        double electric_theta = EM_ASM_DOUBLE({
            return directions.electric.theta;
        });
        double electric_alpha = EM_ASM_DOUBLE({
            return directions.electric.alpha;
        });
        double electric_force = EM_ASM_DOUBLE({
            return directions.electric.force;
        });
        wilsonCloudChamber.setElectricFieldDirection(anglesToVector(electric_theta, electric_alpha));
        wilsonCloudChamber.setElectricFieldCharge(electric_force);

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
                        if (traectories.count(particle)) {
                            if (ticks % 3 == 0)
                                traectories.at(particle).push_back(particle->getCoordinate());
                        }
                        else {
                            vector<Vector> traectory;
                            traectory.push_back(particle->getCoordinate());
                            traectory.push_back(particle->getCoordinate());
                            traectories.insert(particle_pair(particle, traectory));
                        }
                        used.push_back(particle);
                        auto coordinate = particle->getCoordinate();
                        DrawSphere({(float) (coordinate.x() / borders.x() * cubeSize.x()), (float) (coordinate.y() / borders.y() * cubeSize.y()), (float) (coordinate.z() / borders.z() * cubeSize.z())}, 0.1, *colorIt);
                    }
                }
                for (auto mapIt = traectories.begin(); mapIt != traectories.end(); mapIt++) {
                    if (find(used.begin(), used.end(), mapIt->first) == used.end()) {
                        unused.push_back(mapIt->first);
                    }
                    else {
                        auto beginIt = mapIt->second.begin();
                        auto endIt = mapIt->second.begin() + 1;
                        for (; endIt != mapIt->second.end(); beginIt++, endIt++) {
                            DrawLine3D({(float) (beginIt->x() / borders.x() * cubeSize.x()), (float) (beginIt->y() / borders.y() * cubeSize.y()), (float) (beginIt->z() / borders.z() * cubeSize.z())}, {(float) (endIt->x() / borders.x() * cubeSize.x()), (float) (endIt->y() / borders.y() * cubeSize.y()), (float) (endIt->z() / borders.z() * cubeSize.z())}, WHITE);
                            //DrawSphere({(float) (beginIt->x() / borders.x() * cubeSize.x()), (float) (beginIt->y() / borders.y() * cubeSize.y()), (float) (beginIt->z() / borders.z() * cubeSize.z())}, 0.02, WHITE);
                            if (endIt == mapIt->second.end() - 1) {
                                auto *pointCoordinate = &(mapIt->first->getCoordinate());
                                DrawLine3D({(float) (endIt->x() / borders.x() * cubeSize.x()), (float) (endIt->y() / borders.y() * cubeSize.y()), (float) (endIt->z() / borders.z() * cubeSize.z())}, {(float) (pointCoordinate->x() / borders.x() * cubeSize.x()), (float) (pointCoordinate->y() / borders.y() * cubeSize.y()), (float) (pointCoordinate->z() / borders.z() * cubeSize.z())}, WHITE);
                            }
                        }
                    }
                }

                for (auto unusedIt = unused.begin(); unusedIt != unused.end(); unusedIt++) {
                    // if (traectories.at(*unusedIt).size()) {
                    //     auto beginIt = traectories.at(*unusedIt).begin();
                    //     auto endIt = traectories.at(*unusedIt).begin() + 1;
                    //     for (; endIt != traectories.at(*unusedIt).end(); beginIt++, endIt++) {
                    //         DrawLine3D({(float) (beginIt->x() / borders.x() * cubeSize.x()), (float) (beginIt->y() / borders.y() * cubeSize.y()), (float) (beginIt->z() / borders.z() * cubeSize.z())}, {(float) (endIt->x() / borders.x() * cubeSize.x()), (float) (endIt->y() / borders.y() * cubeSize.y()), (float) (endIt->z() / borders.z() * cubeSize.z())}, WHITE);
                    //     }
                    //     if (ticks % 3 == 0)
                    //         traectories.at(*unusedIt).erase(traectories.at(*unusedIt).begin());
                    // }
                    // else
                        traectories.erase(*unusedIt);
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