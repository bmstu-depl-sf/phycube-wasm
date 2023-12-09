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
    camera.target = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera.fovy = 50.0f;
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
        //camera.position = mul(camera.position, 20 / size(camera.position)); 

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
        DrawLine3D({10, 5, 5}, {5, 5, 5}, RED);
        DrawCylinderEx({10, 5, 5}, {10.5, 5, 5}, 0.15, 0, 25, RED);
        DrawLine3D({5, 10, 5}, {5, 5, 5}, GREEN);
        DrawCylinderEx({5, 10, 5}, {5, 10.5, 5}, 0.15, 0, 25, GREEN);
        DrawLine3D({5, 5, 10}, {5, 5, 5}, BLUE);
        DrawCylinderEx({5, 5, 10}, {5, 5, 10.5}, 0.15, 0, 25, BLUE);

        // Рисуем вектор по полученным углам
        DrawLine3D(sum(anglesToVector3(theta, alpha), {5, 5, 5}), {5, 5, 5}, YELLOW);
        DrawCylinderEx(sum(anglesToVector3(theta, alpha), {5, 5, 5}), sum(mul(anglesToVector3(theta, alpha), 5.5/5.0), {5, 5, 5}), 0.15, 0, 25, YELLOW);
        DrawCubeWires(camera.target, 10, 10, 10, WHITE);

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}