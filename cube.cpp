#include "cube.h"
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

void Point::move() {
        speed = sum(speed, boost);
        position = sum(position, speed);
    }

int main(void)
{
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

    std::vector<Point> points;
    float speed = 0.1, angle=0;
    int ticks_after_last_point;

    SetTargetFPS(60);                   // Set our example to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Shot new point if enough time spent
        if (++ticks_after_last_point > 100) {
            ticks_after_last_point = 0;
            angle = (float) GetRandomValue(-30, 30) / 100;
            Vector3 speed3 = {0.0, speed * cosf(angle), speed * sinf(angle)};
            Vector3 boost3 = mul(speed3, 0.1);
            points.push_back(Point(WHITE, 0.15f, {0, -5, 0}, speed3, boost3));
        }
        
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

                // Draw points and delete them if they're out of cube
                for (int i = 0; i < points.size(); i++) {
                    points[i].move();
                    if (fmax(fmax(fabs(points[i].position.x), fabs(points[i].position.y)), fabs(points[i].position.z)) >= 5)
                        points.erase(std::next(points.begin(), i--));
                    else
                        DrawSphere(points[i].position, points[i].radius, points[i].color);
                }
                
                // Fraw cube
                DrawCubeWires((Vector3){0.0f, 0.0f, 0.0f}, 10.0f, 10.0f, 10.0f, WHITE);

                // Draw axes and shot vector
                // DrawLine3D({-5, -5, -5}, {0, -5, -5}, RED);
                // DrawLine3D({-5, -5, -5}, {-5, 0, -5}, GREEN);
                // DrawLine3D({-5, -5, -5}, {-5, -5, 0}, BLUE);

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