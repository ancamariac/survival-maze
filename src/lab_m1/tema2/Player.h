#pragma once
#include <glm/ext/vector_float3.hpp>
#include "lab_m1/tema2/SurvivalMaze.h"

#define MAZE_SIZE 4

namespace m1 {
	class SurvivalMaze;
}

class Player {
	public:
		Player(m1::SurvivalMaze* smaze);
		~Player();
		void Draw();

		void SetPosition(glm::vec3 newPosition);
		void Translate(glm::vec3 delta);
		void SetYRot(float yrot);
		glm::vec3 GetPosition();
	private:
		glm::vec3 position;
		m1::SurvivalMaze* survivalMaze;		
		float YAxisRot;
};