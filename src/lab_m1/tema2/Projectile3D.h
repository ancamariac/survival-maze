#pragma once
#include <iostream>
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "Entity3D.h"
#include "SurvivalMaze.h"

#define PRJ_ALIVE_TIME 2.0f

using namespace m1;

class Projectile3D : public Entity3D
{
    public:
        bool dead;
        float aliveTime;
        Projectile3D(Mesh* mesh, Shader* shader, glm::vec3 position, glm::vec3 target, SurvivalMaze* instance);
        ~Projectile3D();

        void Draw();
        void PointTowards(glm::vec3 worldSpacePoint);
        void Update(float deltaSeconds);
    private:
        Projectile3D();
        Mesh* mesh;
        Shader* shader;
        SurvivalMaze* survInstance;

        glm::vec3 direction;
};