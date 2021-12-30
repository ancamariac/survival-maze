#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace utils
{
    class Camera
    {
        public:
            float distanceToTarget;
            glm::vec3 position;
            glm::vec3 forward;
            glm::vec3 right;
            glm::vec3 up;

         public:
            Camera()
            {
                position    = glm::vec3(0, 2, 5);
                forward     = glm::vec3(0, 0, -1);
                up          = glm::vec3(0, 1, 0);
                right       = glm::vec3(1, 0, 0);
                distanceToTarget = 2;
            }

            Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
            {
                Set(position, center, up);
            }

            ~Camera()
            { }

            void SetDistance(float newDist) {
                float deltaDist = distanceToTarget - newDist;
                this->position = position + forward * deltaDist;
                distanceToTarget = newDist;
            }

            // Update camera
            void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
            {
                this->position = position;
                forward     = glm::normalize(center - position);
                right       = glm::normalize(glm::cross(forward, up));
                this->up    = glm::normalize(glm::cross(right, forward));
                distanceToTarget = glm::length(center - position);
            }

            void MoveForward(float distance)
            {
                glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
                position += dir * distance;
            }

            void TranslateForward(float distance)
            {
                position += forward * distance;
            }

            void TranslateUpward(float distance)
            {
                position += up * distance;
            }

            void TranslateRight(float distance)
            {
                position += right * distance;
            }

            void RotateFirstPerson_OX(float angle)
            {
                glm::vec4 new_fwd = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0);
                forward = glm::normalize(glm::vec3(new_fwd));

                up = glm::normalize(glm::cross(right, forward));
            }

            void RotateFirstPerson_OY(float angle)
            {
                glm::vec4 new_fwd = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
                forward = glm::normalize(glm::vec3(new_fwd));
                new_fwd = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
                right = glm::normalize(glm::vec3(new_fwd));

                up = glm::normalize(glm::cross(right, forward));
            }

            void RotateFirstPerson_OZ(float angle)
            {
                glm::vec4 new_right = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
                right = glm::normalize(glm::vec3(new_right));
                new_right = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0);
                forward = glm::normalize(glm::vec3(new_right));

                up = glm::normalize(glm::cross(right, forward));
            }

            void RotateThirdPerson_OX(float angle)
            {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OX(angle);
                TranslateForward(-distanceToTarget);
            }

            void RotateThirdPerson_OY(float angle)
            {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OY(angle);
                TranslateForward(-distanceToTarget);
            }

            void RotateThirdPerson_OZ(float angle)
            {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OZ(angle);
                TranslateForward(-distanceToTarget);
            }

            glm::mat4 GetViewMatrix()
            {
                return glm::lookAt(position, position + forward, up);
            }

            glm::vec3 GetTargetPosition()
            {
                return position + forward * distanceToTarget;
            }
        };
}  
