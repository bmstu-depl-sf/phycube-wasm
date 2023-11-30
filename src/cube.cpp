#include "functions.h"
#include "chamber.h"
#include <emscripten.h>
#include <algorithm>
#include <random>
#include <map>

using namespace std;
using namespace phycoub;

int getWidth() {
    double width = EM_ASM_DOUBLE({
        return windowSize().width;
    });
    return (int) width * 2;
}

int getHeight() {
    double height = EM_ASM_DOUBLE({
        return windowSize().height;
    });
    return (int) height * 2;
}

int main(void)
{
    // Инициализируем класс камеры Вильсона
    const double dt = 1e-13;
    MyWilsonCloudChamber wilsonCloudChamber;
    Vector borders(1e-4), cubeSize(10);
    wilsonCloudChamber.setBorders(borders);
    wilsonCloudChamber.setDeltaTime(dt);
    wilsonCloudChamber.setSpecificSourcBornPeriod(100 * dt);

    // Инициализируем окно
    InitWindow(getWidth(), getHeight(), TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(FPS);

    // Инициализируем камеру
    Camera camera = { 0 };
    camera.position = (Vector3){ 20.0f, 0.0f, 0.0f };
    camera.target = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera.fovy = 60.0f;

    // Инициализируем случайные цвета
    vector<Color> colors = {RED, GREEN, YELLOW, ORANGE, VIOLET};
    shuffle(colors.begin(), colors.end(), default_random_engine {random_device {}()});

    map<Particle*, vector<Vector>> traectories;
    vector<pair<ParticleOptions, Color>> colorsAssociations;
    uint64_t ticks = 0;

    while (!WindowShouldClose())
    {
        ticks++;
        vector<Particle*> used(0), unused(0);
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        camera.position = sum(mul(sum(camera.position, {-5, -5, -5}), 20 / size(sum(camera.position, {-5, -5, -5}))), {5, 5, 5});
        SetWindowSize(getWidth(), getHeight());

        // Сохраняем позицию камеры в окне
        EM_ASM({
            camera.x = $0;
            camera.y = $1;
            camera.z = $2;
        }, camera.position.x, camera.position.y, camera.position.z);

        // Вытягиваем направление выстрела частиц
        double shoot_theta = EM_ASM_DOUBLE({
            return directions.shoot.theta;
        });
        double shoot_alpha = EM_ASM_DOUBLE({
            return directions.shoot.alpha;
        });
        Vector shootDirection = anglesToVector(shoot_theta, shoot_alpha);
        wilsonCloudChamber.setSpecificSourceDirection(shootDirection);

        // Вытягиваем характеристики магнитного поля
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

        // Вытягиваем характеристики электрического поля
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

        // Проверяем, включена ли пауза
        int paused = EM_ASM_({
            return paused;
        });
        if (!paused)
            wilsonCloudChamber.phyCoub();
        
        // Очищаем экран
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        
        // Отрисовываем частицы
        auto particleGroupList = wilsonCloudChamber.getUniqParticleGroupList();
        for (auto particleGroupIt = particleGroupList.begin(); particleGroupIt != particleGroupList.end(); particleGroupIt++) {
            auto particleGroup = particleGroupIt->get();
            for (auto particleIt = particleGroup->begin(); particleIt != particleGroup->end(); particleIt++) {
                auto particle = particleIt->get();
                Color color = BLACK;
                bool found = false;
                for (auto assocIt = colorsAssociations.begin(); assocIt != colorsAssociations.end(); assocIt++)
                    if ((assocIt->first.m_ == particle->getOptions().m_) && (assocIt->first.q_ == particle->getOptions().q_)) {
                        color = assocIt->second;
                        found = true;
                    }
                if (!found) {
                    color = colors[colorsAssociations.size()];
                    colorsAssociations.push_back({particle->getOptions(), color});
                }
                if (traectories.count(particle)) {
                    if (ticks % 3 == 0)
                        traectories.at(particle).push_back(particle->getCoordinate());
                }
                else {
                    vector<Vector> traectory;
                    traectory.push_back(particle->getCoordinate());
                    traectory.push_back(particle->getCoordinate());
                    traectories.insert({particle, traectory});
                }
                used.push_back(particle);
                auto coordinate = particle->getCoordinate();
                DrawSphere({(float) (coordinate.x() / borders.x() * cubeSize.x()), (float) (coordinate.y() / borders.y() * cubeSize.y()), (float) (coordinate.z() / borders.z() * cubeSize.z())}, 0.1, color);
            }
        }

        // Отрисовываем траектории частиц
        for (auto mapIt = traectories.begin(); mapIt != traectories.end(); mapIt++) {
            if (find(used.begin(), used.end(), mapIt->first) == used.end()) {
                unused.push_back(mapIt->first);
            }
            else {
                auto beginIt = mapIt->second.begin();
                auto endIt = mapIt->second.begin() + 1;
                for (; endIt != mapIt->second.end(); beginIt++, endIt++) {
                    DrawLine3D({(float) (beginIt->x() / borders.x() * cubeSize.x()), (float) (beginIt->y() / borders.y() * cubeSize.y()), (float) (beginIt->z() / borders.z() * cubeSize.z())}, {(float) (endIt->x() / borders.x() * cubeSize.x()), (float) (endIt->y() / borders.y() * cubeSize.y()), (float) (endIt->z() / borders.z() * cubeSize.z())}, WHITE);
                    if (endIt == mapIt->second.end() - 1) {
                        auto *pointCoordinate = &(mapIt->first->getCoordinate());
                        DrawLine3D({(float) (endIt->x() / borders.x() * cubeSize.x()), (float) (endIt->y() / borders.y() * cubeSize.y()), (float) (endIt->z() / borders.z() * cubeSize.z())}, {(float) (pointCoordinate->x() / borders.x() * cubeSize.x()), (float) (pointCoordinate->y() / borders.y() * cubeSize.y()), (float) (pointCoordinate->z() / borders.z() * cubeSize.z())}, WHITE);
                    }
                }
            }
        }
        
        // Удаляем уничтоженные частицы
        for (auto unusedIt = unused.begin(); unusedIt != unused.end(); unusedIt++)
            traectories.erase(*unusedIt);

        // Завершаем отрисовку
        DrawCubeWires(camera.target, cubeSize.x(), cubeSize.y(), cubeSize.z(), WHITE);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}