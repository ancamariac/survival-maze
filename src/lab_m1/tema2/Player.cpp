#include "Player.h"

Player::Player(m1::SurvivalMaze* smaze) {
    survivalMaze = smaze;
    position = smaze->maze->getEntrancePos();
    YAxisRot = 0.0f;
}

Player::~Player() {

}

glm::vec3 Player::GetPosition() {
    return position;
}

void Player::Draw() {
    {
        utils::Camera* camera = survivalMaze->GetCamera();
        std::unordered_map<std::string, Mesh*> meshes = survivalMaze->GetMeshes();
        std::unordered_map<std::string, Shader*> shaders = survivalMaze->GetShaders();

        glm::mat4 head_matrix = glm::mat4(1);
        head_matrix = glm::translate(head_matrix, position);
        head_matrix = glm::translate(head_matrix, glm::vec3(0, 0.6f, 0));
        head_matrix = glm::rotate(head_matrix, YAxisRot, glm::vec3(0, 1, 0));
        head_matrix = glm::scale(head_matrix, glm::vec3(0.3f, 0.3f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Skin"], head_matrix);

        glm::mat4 body_matrix = glm::mat4(1);
        body_matrix = glm::translate(body_matrix, position);
        body_matrix = glm::translate(body_matrix, glm::vec3(0, 0.08f, 0));
        body_matrix = glm::rotate(body_matrix, YAxisRot, glm::vec3(0, 1, 0));
        body_matrix = glm::scale(body_matrix, glm::vec3(0.5f, 0.7f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Tshirt"], body_matrix);

        glm::mat4 leftarm_matrix = glm::mat4(1);
        leftarm_matrix = glm::translate(leftarm_matrix, position);
        leftarm_matrix = glm::rotate(leftarm_matrix, YAxisRot, glm::vec3(0, 1, 0));
        leftarm_matrix = glm::translate(leftarm_matrix, glm::vec3(-0.35f, 0.18f, 0));
        leftarm_matrix = glm::scale(leftarm_matrix, glm::vec3(0.17f, 0.5f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Tshirt"], leftarm_matrix);

        glm::mat4 rightarm_matrix = glm::mat4(1);
        rightarm_matrix = glm::translate(rightarm_matrix, position);
        rightarm_matrix = glm::rotate(rightarm_matrix, YAxisRot, glm::vec3(0, 1, 0));
        rightarm_matrix = glm::translate(rightarm_matrix, glm::vec3(0.35f, 0.18f, 0));
        rightarm_matrix = glm::scale(rightarm_matrix, glm::vec3(0.17f, 0.5f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Tshirt"], rightarm_matrix);

        glm::mat4 lefthand_matrix = glm::mat4(1);
        lefthand_matrix = glm::translate(lefthand_matrix, position);
        lefthand_matrix = glm::rotate(lefthand_matrix, YAxisRot, glm::vec3(0, 1, 0));
        lefthand_matrix = glm::translate(lefthand_matrix, glm::vec3(-0.35f, -0.18f, 0));
        lefthand_matrix = glm::scale(lefthand_matrix, glm::vec3(0.17f, 0.2f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Skin"], lefthand_matrix);

        glm::mat4 righthand_matrix = glm::mat4(1);
        righthand_matrix = glm::translate(righthand_matrix, position);
        righthand_matrix = glm::rotate(righthand_matrix, YAxisRot, glm::vec3(0, 1, 0));
        righthand_matrix = glm::translate(righthand_matrix, glm::vec3(0.35f, -0.18f, 0));
        righthand_matrix = glm::scale(righthand_matrix, glm::vec3(0.17f, 0.2f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Skin"], righthand_matrix);

        glm::mat4 leftleg_matrix = glm::mat4(1);
        leftleg_matrix = glm::translate(leftleg_matrix, position);
        leftleg_matrix = glm::rotate(leftleg_matrix, YAxisRot, glm::vec3(0, 1, 0));
        leftleg_matrix = glm::translate(leftleg_matrix, glm::vec3(-0.12f, -0.65f, 0));
        leftleg_matrix = glm::scale(leftleg_matrix, glm::vec3(0.22f, 0.75f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Jeans"], leftleg_matrix);

        glm::mat4 rightleg_matrix = glm::mat4(1);
        rightleg_matrix = glm::translate(rightleg_matrix, position);
        rightleg_matrix = glm::rotate(rightleg_matrix, YAxisRot, glm::vec3(0, 1, 0));
        rightleg_matrix = glm::translate(rightleg_matrix, glm::vec3(0.12f, -0.65f, 0));
        rightleg_matrix = glm::scale(rightleg_matrix, glm::vec3(0.22f, 0.75f, 0.3f));
        survivalMaze->RenderMeshExt(meshes["box"], shaders["Jeans"], rightleg_matrix);
    }
}

void Player::SetPosition(glm::vec3 newPosition) {
    position = newPosition;
}

void Player::Translate(glm::vec3 delta) {
    SetPosition(position + delta);
}

void Player::SetYRot(float yrot) {
    YAxisRot = yrot;
}