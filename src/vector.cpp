#include "functions.h"
#include <cmath>
#include <vector>
#include <emscripten.h>

int main(void)
{
    // Инициализируем окно
    const int screenWidth = 450;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Инициализируем камеру
    Camera camera = { 0 };
    camera.position = (Vector3){ 20.0f, 0.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera.fovy = 35.0f;
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        // Обвновляем камеру
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        // Получаем позицию камеры из другого окна
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

        // Получаем нужные углы для отрисовки вектора
        double theta = EM_ASM_DOUBLE({
            return directionById($0).theta;
        }, OBJECT);
        double alpha = EM_ASM_DOUBLE({
            return directionById($0).alpha;
        }, OBJECT);

        // Очищаем экран
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        // Рисуем три оси
        DrawLine3D({5, 0, 0}, {0, 0, 0}, RED);
        DrawCylinderEx({5, 0, 0}, {5.5, 0, 0}, 0.15, 0, 25, RED);
        DrawLine3D({0, 5, 0}, {0, 0, 0}, GREEN);
        DrawCylinderEx({0, 5, 0}, {0, 5.5, 0}, 0.15, 0, 25, GREEN);
        DrawLine3D({0, 0, 5}, {0, 0, 0}, BLUE);
        DrawCylinderEx({0, 0, 5}, {0, 0, 5.5}, 0.15, 0, 25, BLUE);

        // Рисуем вектор по полученным углам
        DrawLine3D(anglesToVector3(theta, alpha), {0, 0, 0}, YELLOW);
        DrawCylinderEx(anglesToVector3(theta, alpha), mul(anglesToVector3(theta, alpha), 5.5/5.0), 0.15, 0, 25, YELLOW);

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}