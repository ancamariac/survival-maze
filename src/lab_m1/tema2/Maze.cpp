#include "Maze.h"

#define DUNGEON_WIDTH 11
#define DUNGEON_HEIGHT 8

Maze::Maze(m1::SurvivalMaze* smaze) {
    survivalMaze = smaze;
    GenerateMap();
    DisplayInConsole();
}

void Maze::UpdateKillingTimes(float delta) {

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (killingTimes[y][x] > 0.0f) {
                
                killingTimes[y][x] -= delta;
                if (killingTimes[y][x] < 0.0f) {
                    walls[y][x] = MazeCellType::Empty;
                    killingTimes[y][x] = 0.0f;
                }
            }
        }
    }
}

void Maze::DisplayInConsole() {
    std::cout << std::endl;
    std::cout << "============= MAZE MATRIX =============" << std::endl;

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            std::cout << int(walls[y][x]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void Maze::GenerateEnemies() {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (walls[y][x] == MazeCellType::Empty) {
                if ((y % 2 == 0) && (x % 2 == 0) && ((rand() % MAZE_HEIGHT) % 2 == 0)) {
                    walls[y][x] = MazeCellType::Enemy;
                }
            }   
        }
    }
}

void Maze::GenerateDungeons() {
    dungeonY = rand() % 3 + 3;
    dungeonX = rand() % 3 + 3;

    for (int i = dungeonY; i < dungeonY + DUNGEON_HEIGHT; i++) {
        for (int j = dungeonX; j < dungeonX + DUNGEON_WIDTH; j++) {
            walls[i][j] = MazeCellType::Empty;
        }
    }

    // random exit betweem dungeonX + 2 and dungeonX + DUNGEON_WIDTH - 2
    exitDungeonX = rand() % (DUNGEON_WIDTH - 4) + (dungeonX + 2);

    // border for the dungeon
    for (int i = dungeonY; i < dungeonY + DUNGEON_HEIGHT + 1; i++) {
        for (int j = dungeonX; j < dungeonX + DUNGEON_WIDTH + 1; j++) {
            if ((i == dungeonY) || (i == dungeonY + DUNGEON_HEIGHT)
                || (j == dungeonX) || (j == dungeonX + DUNGEON_WIDTH)) {
                walls[i][j] = MazeCellType::BorderKey;
            }
        }
    }

    for (int i = dungeonY + 1; i < dungeonY + DUNGEON_HEIGHT; i++) {
        for (int j = dungeonX + 1; j < dungeonX + DUNGEON_WIDTH; j++) {
            if ((i == dungeonY + 1) || (i == dungeonY + DUNGEON_HEIGHT - 1)
                || (j == dungeonX + 1) || (j == dungeonX + DUNGEON_WIDTH - 1)) {
                walls[i][j] = MazeCellType::WallKey;
            }
        }
    }

    //GenerateDungeonEnemies(dungeonX, dungeonY);

    walls[dungeonY + DUNGEON_HEIGHT - 1][exitDungeonX] = MazeCellType::DungeonDoorKey;

}

void Maze::GenerateMap() {

    // Walls everywhere
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            walls[y][x] = MazeCellType::Wall;
        }
    }

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            killingTimes[y][x] = 0.0f;
        }
    }

    // Hunt-and-kill
    int cellX, cellY;
    cellX = rand() % (MAZE_WIDTH - 2) + 1;
    cellY = rand() % MAZE_HEIGHT;

    walls[0][cellX] = MazeCellType::Empty;

    HuntAndKillRoutine(cellX, 0);

    // iesire = x:cellX, y:0
    exitX = cellX;
    exitY = cellY;
    
    // extra iterations
    for (int i = 0; i < 500; i++) {
        cellX = rand() % MAZE_WIDTH;
        cellY = rand() % MAZE_HEIGHT;
        if (walls[cellY][cellX] == MazeCellType::Empty) {
            HuntAndKillRoutine(cellX, cellY);
        }
    }

    // break an exit point

    int randomStartPoint = rand() % MAZE_WIDTH;

    bool foundExit = false;

    for (int i = randomStartPoint; i >= 0 ; i--) {

        if (walls[MAZE_HEIGHT - 2][i] == MazeCellType::Empty) {
            walls[MAZE_HEIGHT - 1][i] = MazeCellType::Empty;
            foundExit = true;
            // intrare = x: i, y:MAZE_HEIGHT - 1
            startX = i;
            startY = MAZE_HEIGHT - 1;
            break;
        }
        
    }

    if (!foundExit) {
        for (int i = MAZE_WIDTH - 1; i > randomStartPoint; i--) {

            if (walls[MAZE_HEIGHT - 2][i] == MazeCellType::Empty) {
                walls[MAZE_HEIGHT - 1][i] = MazeCellType::Empty;
                // iesire = x: i, y:MAZE_HEIGHT - 1
                startX = i;
                startY = MAZE_HEIGHT - 1;
                break;
            }
        }
    }

    //GenerateDungeons(); 

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (walls[y][x] == MazeCellType::BorderKey ||
                walls[y][x] == MazeCellType::DungeonDoorKey) {
                walls[y][x] = MazeCellType::Empty;
            }
            if (walls[y][x] == MazeCellType::WallKey) {
                walls[y][x] = MazeCellType::Wall;
            }
        }
    }

    GenerateEnemies();
}

int dirs[] = { 0, 1,  // sus     0
               0, -1, // jos    1
               -1, 0, // stanga 2
               1, 0,  // dreapta 3
};

int rightNeighbours[] = {
    3,
    2,
    0,
    1
};

// must start from a 0 ( empty cell )
void Maze::HuntAndKillRoutine(int x, int y) {
    int lastPunched = 0;
    int chosenDirections = 0;

    while (true) {

        int chosenDirection = rand() % 4;
        int dirX = dirs[chosenDirection * 2];
        int dirY = dirs[chosenDirection * 2 + 1];

        if (y + dirY <= 0 || y + dirY >= MAZE_HEIGHT-1 || 
            x + dirX <= 0 || x + dirX >= MAZE_WIDTH-1) {
            chosenDirections++;
            if (chosenDirections == 10) {
                return;
            }
            continue;
        }
        chosenDirections = 0;
        int rightDirection = rightNeighbours[chosenDirection];

        int rDirX = dirs[rightDirection * 2];

        int rDirY = dirs[rightDirection * 2 + 1];

        
        // short circuit to prevent out of bounds

        bool beyond = false, right = false, left = false;

        if (y + dirY*2 <= 0 || y + dirY*2 >= MAZE_HEIGHT - 1 ||
            x + dirX*2 <= 0 || x + dirX*2 >= MAZE_WIDTH - 1) {
            beyond = true;
        }

        if (y + dirY+rDirY <= 0 || y + dirY+ rDirY >= MAZE_HEIGHT - 1 ||
            x + dirX+ rDirX <= 0 || x + dirX+ rDirX >= MAZE_WIDTH - 1) {
            right = true;
        }

        if (y + dirY - rDirY <= 0 || y + dirY - rDirY >= MAZE_HEIGHT - 1 ||
            x + dirX - rDirX <= 0 || x + dirX - rDirX >= MAZE_WIDTH - 1) {
            left = true;
        }

        if (walls[y+dirY][x+dirX] == MazeCellType::Wall // if the next cell is a wall
            && (beyond || walls[y+dirY*2][x+dirX*2] == MazeCellType::Wall) // if the next cell's "beyond" neighbour is a wall
            && (right || walls[y+dirY+rDirY][x+dirX+rDirX] == MazeCellType::Wall) // if the next cell's "right" neighbour is a wall
            && (left || walls[y+dirY-rDirY][x+dirX-rDirX] == MazeCellType::Wall) // if the next cell's "left" neighbour is a wall
            ) {
            walls[y + dirY][x + dirX] = MazeCellType::Empty;

            y += dirY;
            x += dirX;
            lastPunched = 0;
        }
        else {

            lastPunched += 1;
        }

        if (lastPunched == 30) {

            break;
        }
    }
}

Maze::~Maze() {

}

float t = 0;

void Maze::Draw(float deltaTimeSeconds) {  
    
    utils::Camera* camera = survivalMaze->GetCamera();
    std::unordered_map<std::string, Mesh*> meshes = survivalMaze->GetMeshes();
    std::unordered_map<std::string, Shader*> shaders = survivalMaze->GetShaders();

    t += deltaTimeSeconds;

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            // Draw maze walls
            if (walls[y][x] == MazeCellType::Wall) {

                glm::mat4 translateCube = glm::translate(glm::mat4(1), glm::vec3(x * WALL_WIDTH, WALL_HEIGHT/2, y * WALL_WIDTH));
                glm::mat4 scaleCube = glm::scale(translateCube, glm::vec3(WALL_WIDTH, WALL_HEIGHT, WALL_WIDTH));
                survivalMaze->RenderSimpleMesh(meshes["box"], shaders["WallShader"], scaleCube, survivalMaze->mapTextures["wall"]);
            }
            // Enemies
            if (walls[y][x] == MazeCellType::Enemy) {

                srand(y * MAZE_WIDTH + x);
                float phase = float(rand() % 1000) / 1000.0f;
                phase *= PI;

                // speed intre 70 si 100% din speed ul initial
                float temp = float(rand() % 300 + 700) / 1000.0f;
                float speedup = 5.f * temp;


                glm::mat4 translateEnemy = glm::translate(glm::mat4(1), glm::vec3(x * WALL_WIDTH, ENEMY_SCALE / 2, y * WALL_WIDTH));

                // Movement of enemies - Lissajous curve
                translateEnemyX = 1 * sin(omegaX * t * speedup + phase);
                translateEnemyZ = 1 * sin(omegaY * t * speedup + phase);

                glm::mat4 moveEnemy = glm::translate(translateEnemy, glm::vec3(translateEnemyX, 0, translateEnemyZ));
                glm::mat4 scaleEnemy = glm::scale(moveEnemy, glm::vec3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE));


                shaders["Enemy"]->Use();
                int loc_distortionScale = glGetUniformLocation(shaders["Enemy"]->program, "distortionScale");

                if (killingTimes[y][x] > 0.0f) {
                    glUniform1f(loc_distortionScale, (0.5f - killingTimes[y][x]) * 2.0f);
                }
                else {
                    glUniform1f(loc_distortionScale, 0.0f);
                }
                
                survivalMaze->RenderMeshExt(meshes["sphere"], shaders["Enemy"], scaleEnemy);
            }
        }
    }   
}

glm::vec3 Maze::getWorldPosAtGrid(int x, int y) {
    return glm::vec3(float (x * WALL_WIDTH), 1.0f, float(y * WALL_WIDTH));
}

float Maze::getWorldPosAtGridX(int x) {
    return float(x * WALL_WIDTH);
}

float Maze::getWorldPosAtGridZ(int z) {
    return float(z * WALL_WIDTH);
}

glm::vec3 Maze::getEntrancePos() {
    return getWorldPosAtGrid(startX, startY);
}

glm::vec3 Maze::getExitPos() {
    return getWorldPosAtGrid(exitX, exitY);
}