#include "lab_m1/tema2/SurvivalMaze.h"

#include <vector>
#include <string>
#include <iostream>
#include <lab_m1/tema2/Projectile3D.h>

using namespace std;
using namespace m1;

std::vector<Projectile3D*> projectiles;

SurvivalMaze::SurvivalMaze()
{
}

SurvivalMaze::~SurvivalMaze()
{
}

void SurvivalMaze::Init()
{
    maze = new Maze(this);
    player = new Player(this);

    window->DisablePointer();

    renderCameraTarget = true;

    camera = new utils::Camera();
    
    HUDcamera = new utils::Camera();

    camera->Set(glm::vec3(0, 3, 3), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Initial position of the camera - start of the maze
    camera->MoveForward(-maze->startY * 4);
    camera->TranslateRight(maze->startX * 4);

    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN("src/lab_m1/tema2/textures/wall.png").c_str(), GL_REPEAT);
        mapTextures["wall"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN("src/lab_m1/tema2/textures/ground3.jpg").c_str(), GL_REPEAT);
        mapTextures["ground3"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN("src/lab_m1/tema2/textures/ground1.jpg").c_str(), GL_REPEAT);
        mapTextures["ground1"] = texture;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
   
    {
        Shader* shader = new Shader("WallShader");
        shader->AddShader(PATH_JOIN("src/lab_m1/tema2/shaders/Maze/VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN("src/lab_m1/tema2/shaders/Maze/FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("FloorShader");
        shader->AddShader("src/lab_m1/tema2/shaders/Floor/VertexShader.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/Floor/FragmentShader.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Background");
        shader->AddShader("src/lab_m1/tema2/shaders/HUD/BackgroundVS.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/HUD/BackgroundFS.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("HealthBar");
        shader->AddShader("src/lab_m1/tema2/shaders/HUD/HealthVS.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/HUD/HealthFS.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TimeBar");
        shader->AddShader("src/lab_m1/tema2/shaders/HUD/TimeVS.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/HUD/TimeFS.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Enemy");
        shader->AddShader("src/lab_m1/tema2/shaders/Enemy/VertexShader.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/Enemy/FragmentShader.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Skin");
        shader->AddShader("src/lab_m1/tema2/shaders/Player/SkinVS.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/Player/SkinFS.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Tshirt");
        shader->AddShader("src/lab_m1/tema2/shaders/Player/TshirtVS.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/Player/TshirtFS.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Jeans");
        shader->AddShader("src/lab_m1/tema2/shaders/Player/JeansVS.glsl", GL_VERTEX_SHADER);
        shader->AddShader("src/lab_m1/tema2/shaders/Player/JeansFS.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 10.0f);
    projection = true;
}

void SurvivalMaze::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void SurvivalMaze::Update(float deltaTimeSeconds)
{
    timeLeft -= timeDecrease;

    // Draw the maze
    maze->Draw(deltaTimeSeconds);

    // Draw the player (third person perspective)
    if (renderCameraTarget) {
        player->Draw();
    }

    // Render the plane  
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(40, -0.01f, 40));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0, 2));
        RenderSimpleMesh(meshes["plane50"], shaders["WallShader"], modelMatrix, mapTextures["ground1"]);
    }

    /* Debug block
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0, 86));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        RenderSimpleMesh(meshes["box"], shaders["WallShader"], modelMatrix, mapTextures["haha"]);
    }*/

    // Draw the HUD
    {
        //  ==== Health bar ====
        glm::mat4 health_bar = glm::mat4(1);
        health_bar = glm::translate(health_bar, glm::vec3(180, window->GetResolution().y - 50, 0));

        health_bar = glm::translate(health_bar, glm::vec3(-150, -30, 0));
        health_bar = glm::scale(health_bar, glm::vec3(playerHP, 1, 0));
        health_bar = glm::translate(health_bar, glm::vec3(150, 30, 0));

        health_bar = glm::scale(health_bar, glm::vec3(145, 25, 0));
        RenderMeshHUDExt(meshes["quad"], shaders["HealthBar"], health_bar);

        // Background health bar
        glm::mat4 background_health = glm::mat4(1);
        background_health = glm::translate(background_health, glm::vec3(180, window->GetResolution().y - 50, 0));
        background_health = glm::scale(background_health, glm::vec3(145, 25, 0));
        RenderMeshHUDExt(meshes["quad"], shaders["Background"], background_health);

        // Frame health bar
        glm::mat4 frame_health = glm::mat4(1);
        frame_health = glm::translate(frame_health, glm::vec3(180, window->GetResolution().y - 50, 0));
        frame_health = glm::scale(frame_health, glm::vec3(150, 30, 0));
        RenderMeshHUDExt(meshes["quad"], shaders["HealthBar"], frame_health);


        // ==== Time bar ====
        glm::mat4 time_bar = glm::mat4(1);
        time_bar = glm::translate(time_bar, glm::vec3(180, window->GetResolution().y - 120, 0));

        time_bar = glm::translate(time_bar, glm::vec3(-150, -30, 0));
        time_bar = glm::scale(time_bar, glm::vec3(timeLeft, 1, 0));
        time_bar = glm::translate(time_bar, glm::vec3(150, 30, 0));

        time_bar = glm::scale(time_bar, glm::vec3(145, 25, 0));      
        RenderMeshHUDExt(meshes["quad"], shaders["TimeBar"], time_bar);

        // Background time bar
        glm::mat4 background_time = glm::mat4(1);
        background_time = glm::translate(background_time, glm::vec3(180, window->GetResolution().y - 120, 0));
        background_time = glm::scale(background_time, glm::vec3(145, 25, 0));
        RenderMeshHUDExt(meshes["quad"], shaders["Background"], background_time);

        // Frame time bar
        glm::mat4 frame_time = glm::mat4(1);
        frame_time = glm::translate(frame_time, glm::vec3(180, window->GetResolution().y - 120, 0));
        frame_time = glm::scale(frame_time, glm::vec3(150, 30, 0));
        RenderMeshHUDExt(meshes["quad"], shaders["TimeBar"], frame_time);
    }

    maze->UpdateKillingTimes(deltaTimeSeconds);

    // Projectiles
    for (vector<Projectile3D*>::iterator i = projectiles.begin(); i != projectiles.end(); ) {

        Projectile3D* proj = *i;

        int bulletZ = 0;
        int bulletX = 0;
        int bulletY = 0;

        if (!proj->dead) {
            proj->Update(deltaTimeSeconds);
            proj->Draw();
            i++;

            // Collision between projectiles and enemy
            bulletZ = (round)((proj->GetPosition().z) / WALL_WIDTH);
            bulletX = (round)((proj->GetPosition().x) / WALL_WIDTH);
            bulletY = (round)(proj->GetPosition().y);

            if (maze->walls[bulletZ][bulletX] == MazeCellType::Enemy &&
                bulletY <= ENEMY_SCALE) {

                if (maze->killingTimes[bulletZ][bulletX] == 0.0f) {
                    maze->killingTimes[bulletZ][bulletX] = 0.5f; // How many seconds till the enemy gets killed
                }
                

                proj->dead = true;
            }
        }
        else {
            delete proj;
            i = projectiles.erase(i);
        }
    }
    
    initialPositionX = maze->startX;
    initialPositionY = maze->startY;  
    
    // Mission accomplished - you solved the maze
    if ((player->GetPosition().z / WALL_WIDTH < EXIT_POINT)) {
        cout << " =========  GOOD JOB! ========= " << endl;
        exit(0);
    }

    // Better luck next time, the player couldn't find the exit
    if ((timeLeft <= 0) || (playerHP <= 0)) {
        cout << " =========  GAME OVER! ========= " << endl;
        exit(0);
    }

}

void SurvivalMaze::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

std::unordered_map<std::string, Mesh*>& SurvivalMaze::GetMeshes() {
    return meshes;
}

std::unordered_map<std::string, Shader*>& SurvivalMaze::GetShaders() {
    return shaders;
}

utils::Camera* SurvivalMaze::GetCamera() {
    return camera;
}

void SurvivalMaze::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform1i(glGetUniformLocation(shader->program, "mixTheTextures"), mixTextures);

    if (texture1)
    {
        // - activate texture location 0
        glActiveTexture(GL_TEXTURE0);

        // - bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

        // - send theuniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        // - activate texture location 1
        glActiveTexture(GL_TEXTURE1);

        // - bind the texture2 ID
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());

        // - send the uniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void SurvivalMaze::RenderMeshExt(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    RenderMesh(mesh, shader, modelMatrix);
}

void SurvivalMaze::RenderMeshHUDExt(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    RenderMeshHUD(mesh, shader, modelMatrix);
}

void SurvivalMaze::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void SurvivalMaze::RenderMeshHUD(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
       return;

    projectionMatrix = glm::ortho(0.0f, (float)window->GetResolution().x, 0.0f, (float)window->GetResolution().y, -5.0f, 100.0f);
    glm::mat4 modelMatrixIdentity = glm::mat4(1);
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrixIdentity));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void SurvivalMaze::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 10.0f;

    if (window->KeyHold(GLFW_KEY_W)) {

        float dist = deltaTime * cameraSpeed;
        camera->MoveForward(dist);
        
        glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
        player->Translate(dir * dist);
        
        // Collision:

        int playerX;
        int playerZ;

        // FRONT
        if (dir.z < 0) {
            playerZ = (round)((player->GetPosition().z - PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }
        // BACK
        else {
            playerZ = (round)((player->GetPosition().z + PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }
        // LEFT
        if (dir.x < 0) {
            playerX  = (round)((player->GetPosition().x - PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }
        // RIGHT
        else {
            playerX = (round)((player->GetPosition().x + PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Enemy) {
            playerHP -= 0.1f;
            maze->walls[playerZ][playerX] = MazeCellType::Empty;
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Wall) {
            player->Translate(-dir * (dist));
            camera->MoveForward(-(dist));
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {

        float dist = deltaTime * cameraSpeed;
        camera->TranslateRight(-dist);

        // Should the camera->right vec3 be normalized?
        glm::vec3 dir = glm::normalize(glm::vec3(-camera->right.x, 0, -camera->right.z));
        player->Translate(-camera->right * dist);

        int playerZ;
        int playerX;

        // FRONT
        if (dir.z < 0) {
            playerZ = (round)((player->GetPosition().z - PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }
        // BACK
        else {
            playerZ = (round)((player->GetPosition().z + PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }
        // LEFT
        if (dir.x < 0) {
            playerX = (round)((player->GetPosition().x - PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }
        // RIGHT
        else {
            playerX = (round)((player->GetPosition().x + PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Enemy) {
            playerHP -= 0.1f;
            maze->walls[playerZ][playerX] = MazeCellType::Empty;
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Wall) {
            player->Translate(camera->right * dist);
            camera->TranslateRight(dist);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {

        float dist = deltaTime * cameraSpeed;
        camera->MoveForward(-dist);

        glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
        player->Translate(-dir * dist);

        // Collision:

        int playerX;
        int playerZ;

        // FRONT
        if (dir.z < 0) {
            playerZ = (round)((player->GetPosition().z + PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }
        // BACK
        else {
            playerZ = (round)((player->GetPosition().z - PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }
        // LEFT
        if (dir.x < 0) {
            playerX = (round)((player->GetPosition().x + PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }
        // RIGHT
        else {
            playerX = (round)((player->GetPosition().x - PLAYER_DEPTH_FACTOR) / WALL_WIDTH);
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Enemy) {
            playerHP -= 0.1f;
            maze->walls[playerZ][playerX] = MazeCellType::Empty;
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Wall) {
            player->Translate(dir * (dist));
            camera->MoveForward(dist);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {

        float dist = deltaTime * cameraSpeed;
        camera->TranslateRight(dist);

        // Should the camera->right vec3 be normalized?
        glm::vec3 dir = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));
        player->Translate(camera->right * dist);

        int playerZ;
        int playerX;

        // FRONT
        if (dir.z < 0) {
            playerZ = (round)((player->GetPosition().z - PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }
        // BACK
        else {
            playerZ = (round)((player->GetPosition().z + PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }
        // LEFT
        if (dir.x < 0) {
            playerX = (round)((player->GetPosition().x - PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }
        // RIGHT
        else {
            playerX = (round)((player->GetPosition().x + PLAYER_WIDTH_FACTOR / 2) / WALL_WIDTH);
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Enemy) {
            playerHP -= 0.1f;
            maze->walls[playerZ][playerX] = MazeCellType::Empty;
        }

        if (maze->walls[playerZ][playerX] == MazeCellType::Wall) {
            player->Translate(-camera->right * dist);
            camera->TranslateRight(-dist);
        }
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(-deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(deltaTime * cameraSpeed);
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

        if (renderCameraTarget == true) {
            camera->SetDistance(0.0f);
            camera->position = camera->position + glm::vec3(0, 1, 0);
        }
        renderCameraTarget = false;
        
    }
    else {
        if (renderCameraTarget == false) {
            camera->SetDistance(4.0f);
            camera->position = camera->position - glm::vec3(0, 1, 0);
        }
        renderCameraTarget = true;       
    }
}

void SurvivalMaze::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (!renderCameraTarget) {
        camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
    } else {
        camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
    }
    
    // proiectam forward camera direction pe planul XOZ
    glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));

    // calculam unghiul theta intre dir si (0,0,-1)
    // (0, 0, -1) - pentru ca vrem sa-l privim din spate
    Yrot = acos(glm::dot(dir, glm::vec3(0, 0, -1)));

    // determinam daca unghiul este cu plus sau cu minus
    glm::vec3 cross = glm::cross(dir, glm::vec3(0, 0, -1));

    if (cross.y > 0) {
        Yrot *= -1;
    }

    player->SetYRot(Yrot); 
}

void SurvivalMaze::OnKeyPress(int key, int mods)
{ 
    if (key == GLFW_KEY_O)
    {
        playerHP += 0.01f;
    }
    if (key == GLFW_KEY_P)
    {
        timeLeft += 0.01f;
    }
}

void SurvivalMaze::OnKeyRelease(int key, int mods)
{
}

void SurvivalMaze::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

        int playerX = (round)(player->GetPosition().x / WALL_WIDTH);
        int playerZ = (round)(player->GetPosition().z / WALL_WIDTH);

        if (button == 1) { 
            
            Projectile3D* projectile = new Projectile3D(meshes["box"], shaders["VertexNormal"],
                camera->position + camera->forward * 1.0f, camera->position + camera->forward * 5.0f, this);
            projectile->SetScale(glm::vec3(0.2f, 0.2f, 1.0f));
            projectiles.push_back(projectile);
        }
    }
}