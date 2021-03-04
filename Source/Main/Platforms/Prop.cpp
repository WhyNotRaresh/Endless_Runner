#include "Prop.h"
#include "Platform.h"

using namespace Prop;

OnPlatformProp::OnPlatformProp(float position, bool hardness, std::string meshName) :
	position(position), hard(hardness), meshName(meshName) {};

OnPlatformProp::~OnPlatformProp() {};

bool OnPlatformProp::IsHard() {
	return this->hard;
}

std::string OnPlatformProp::GetMeshName() {
	return this->meshName;
}

glm::vec3 OnPlatformProp::GetCollisionPoint(glm::vec3 platformPosition) {
	return platformPosition + glm::vec3(0, 1, (0.5 - this->position) * Platform::platformLength * Platform::scale.z);
}

glm::mat4 OnPlatformProp::ComputeModelMatrix(glm::vec3 platformPosition) {
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1), platformPosition);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, (0.5 - this->position) * Platform::platformLength * Platform::scale.z));
	if (hard) {
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(45), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(90), glm::vec3(1, 0, 0));
	} else {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
	}
	return modelMatrix;
}

/*
	Prop Factory
*/
PropFactory* PropFactory::instance = 0;					// class instance

PropFactory::PropFactory() {};

PropFactory::~PropFactory() {};

PropFactory* PropFactory::GetInstance() {
	if (PropFactory::instance == NULL) {
		instance = new PropFactory();
	}
	return instance;
}

OnPlatformProp* PropFactory::SpawnRandomProp() {
	int random = rand() % 5;

	if (random == 0) {
		return new OnPlatformProp(0.1 + (float)(rand() % 9) / 10.0, false, "cilinder");
	} else if (random == 1) {
		return new OnPlatformProp(0.1 + (float)(rand() % 9) / 10.0, true, "pyramid");
	}

	return NULL;
}