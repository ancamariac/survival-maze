#include "Entity3D.h"

#include <iostream>
using namespace std;

Entity3D::Entity3D() {
    // Setting default values
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f);
    rotationPivot = glm::vec3(0.0f);

    dirtyTransform = true;
}

Entity3D::~Entity3D() {

}

void Entity3D::Translate(glm::vec3 delta) {
    SetPosition(position + delta);
}

void Entity3D::SetPosition(glm::vec3 newPosition) {
    position = newPosition;
    dirtyTransform = true;
}

glm::vec3 Entity3D::GetPosition() {
    return position;
}

void Entity3D::SetRotation(glm::vec3 rotation) {
    this->rotation = rotation;
    dirtyTransform = true;
}

void Entity3D::SetScale(glm::vec3 scale) {
    this->scale = scale;
    dirtyTransform = true;
}

void Entity3D::UpdateTransformMatrix() {
    if (dirtyTransform)
    {
        glm::vec3 centre = position - rotationPivot;
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), centre);

        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), this->rotation.x, glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), this->rotation.y, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), this->rotation.z, glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 moveInRotationPivot = glm::translate(glm::mat4(1.0f), -rotationPivot);
        glm::mat4 moveOutRotationPivot = glm::translate(glm::mat4(1.0f), +rotationPivot);

        // rotate around the rotation pivot
        glm::mat4 rotation =  moveOutRotationPivot*rotationZ*rotationY*rotationX*moveInRotationPivot;

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
        transformMatrix = translation * rotation * scale;

        dirtyTransform = false;
    }
}

glm::mat4 Entity3D::GetTransformMatrix() {
    return transformMatrix;
}

void Entity3D::SetRotationPivot(glm::vec3 pivot) {
    rotationPivot = pivot;
    dirtyTransform = true;
}