#pragma once
#include <glm/ext/vector_float3.hpp>
#include "lab_m1/tema2/SurvivalMaze.h"
#include <iostream>
#include <vector>

#define MAZE_WIDTH 20
#define MAZE_HEIGHT 20
#define WALL_HEIGHT 5.0f
#define WALL_WIDTH 4.0f
#define ENEMY_SCALE 3.0f

#define PI 3.14f

using namespace std;

class WallBlock;

enum class MazeCellType {
	Empty = 0,
	Wall = 1,
	Enemy = 2,
	DungeonDoorKey = 3,
	BorderKey = 5,
	WallKey = 6,
};

namespace m1 {
	class SurvivalMaze;
}

class Maze {
	public:

		int startX, startY, exitX, exitY;
		int dungeonX = 0, dungeonY = 0, exitDungeonX = 0, exitDungeonY = 0;
		float translateEnemyX = 0;
		float translateEnemyZ = 0;

		float omegaX = 2;
		float omegaY = 1;

		Maze(m1::SurvivalMaze* smaze);
		~Maze();
		void Draw(float deltaTimeSeconds);
		glm::vec3 getWorldPosAtGrid(int x, int y);
		float getWorldPosAtGridX(int x);
		float getWorldPosAtGridZ(int z);
		glm::vec3 getEntrancePos();
		glm::vec3 getExitPos();

		MazeCellType walls[MAZE_HEIGHT][MAZE_WIDTH];
		float killingTimes[MAZE_HEIGHT][MAZE_WIDTH];

		void UpdateKillingTimes(float delta);

	private:
		m1::SurvivalMaze* survivalMaze;
		void GenerateMap();
		void GenerateDungeons();
		void GenerateEnemies();
		void HuntAndKillRoutine(int x, int y);
		void DisplayInConsole();
};