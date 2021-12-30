#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


class Entity3D
{
    public:
        Entity3D();
        ~Entity3D();

        void Translate(glm::vec3 delta);
        void SetPosition(glm::vec3 newPosition);
        glm::vec3 GetPosition();
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);
        void UpdateTransformMatrix();
        glm::mat4 GetTransformMatrix();

        void SetRotationPivot(glm::vec3 pivot);
    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 rotationPivot;

        bool dirtyTransform;
        glm::mat4 transformMatrix;
};