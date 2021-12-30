#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"
#include "components/transform.h"
#include "lab_m1/tema2/Player.h"
#include "lab_m1/tema2/Maze.h"
#include <glm/ext/vector_float3.hpp>

#define zNear 0.02f
#define zFar 300.0f
#define EXIT_POINT -1
#define PLAYER_HEIGHT_FACTOR 1.75f
#define PLAYER_WIDTH_FACTOR 0.84f
#define PLAYER_DEPTH_FACTOR 0.3f


class Player;
class Maze;

namespace m1
{
    class SurvivalMaze : public gfxc::SimpleScene
    {
        public:
            SurvivalMaze();
            ~SurvivalMaze();

            void Init() override;
            void RenderMeshExt(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
            void RenderMeshHUDExt(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
            void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
           
            utils::Camera* GetCamera();

            std::unordered_map<std::string, Mesh*>& GetMeshes();
            std::unordered_map<std::string, Shader*>& GetShaders();
            std::unordered_map<std::string, Texture2D*> mapTextures;
        
            int initialPositionX = 0;
            int initialPositionY = 0;

            Maze* maze;

            bool mixTextures;

        private:
           
            void FrameStart() override;
            void Update(float deltaTimeSeconds) override;
            void FrameEnd() override;

            void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
            void RenderMeshHUD(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
            void OnInputUpdate(float deltaTime, int mods) override;
            void OnKeyPress(int key, int mods) override;
            void OnKeyRelease(int key, int mods) override;
            void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
            void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

        protected:
            utils::Camera* camera;
            utils::Camera* HUDcamera;
            glm::mat4 projectionMatrix;

            bool projection;
            bool renderCameraTarget;

            float playerHP = 1;
            float timeLeft = 1;
            float timeDecrease = 0.0001;
            float time = 0;
            float Yrot;

            Player * player;
        };
}   
