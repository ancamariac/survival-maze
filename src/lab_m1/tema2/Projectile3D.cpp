#include "Projectile3D.h"

using namespace std;

Projectile3D::Projectile3D(Mesh* mesh, Shader* shader, glm::vec3 in_position, glm::vec3 target, SurvivalMaze* instance)
{
	dead = false;
	aliveTime = 0;

	this->mesh = mesh;
	this->shader = shader;
	survInstance = instance;

	SetRotationPivot(glm::vec3(0, 0, 0));

	SetPosition(in_position);

	// Se normalizeaza pentru ca vrem doar directia, nu ne intereseaza magnitudinea.
	direction = glm::normalize(target - in_position);

	PointTowards(target);
}

Projectile3D::~Projectile3D()
{
}

void Projectile3D::Draw() {
	this->UpdateTransformMatrix();
	survInstance->RenderMeshExt(mesh, shader, this->GetTransformMatrix());
}

void Projectile3D::PointTowards(glm::vec3 worldSpacePoint) {
	glm::vec3 position = GetPosition();

	glm::mat4 m = glm::inverse(glm::lookAt(glm::vec3(0,0,0), worldSpacePoint - position,
		glm::vec3(0.0f, 1.0f, 0.0f)));
	
	glm::quat q = toQuat(m);

	glm::vec3 rot = glm::vec3(
		glm::pitch(q),
		glm::yaw(q),
		glm::roll(q)
	);

	SetRotation(rot);
}

void Projectile3D::Update(float deltaSeconds) {
	aliveTime += deltaSeconds;

	if (aliveTime > PRJ_ALIVE_TIME) {
		dead = true;
	}

	static const float projectileSpeed = 10.0f;

	Translate(direction * deltaSeconds * projectileSpeed);
}
