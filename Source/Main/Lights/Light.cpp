#include "Light.h"

using namespace Light;

/*
	Light Class methods
*/

glm::vec3 LightClass::ambientIntensity = glm::vec3(0);
float LightClass::scale = 0.05;

LightClass::LightClass(glm::vec3 lightIntensity, glm::vec3 position) :
	lightIntensity(lightIntensity), pos(position) { };


LightClass::LightClass(glm::vec3 lightIntensity, glm::vec3 position, glm::vec3 speed, MaterialProperties material) :
	lightIntensity(lightIntensity), pos(position), speed(speed), material(material), isSpotLight(false) {};

LightClass::LightClass(glm::vec3 lightIntensity, glm::vec3 position, glm::vec3 speed, MaterialProperties material, spotlight prop) :
	lightIntensity(lightIntensity), pos(position), speed(speed), material(material), spotLightProp(prop), isSpotLight(true) { };

LightClass::~LightClass() { };

glm::vec3 LightClass::GetAmbientIntensity() {
	return LightClass::ambientIntensity;
}

glm::vec3 LightClass::GetLightIntensity() {
	return this->lightIntensity;
}

glm::vec3 LightClass::GetPosition() {
	return this->pos;
}

glm::mat4 LightClass::GetModelMatrix() {
	glm::mat4 modelMatrix(1);
	modelMatrix = glm::translate(modelMatrix, this->pos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
	return modelMatrix;
}

MaterialProperties LightClass::GetMaterial() {
	return this->material;
}

spotlight LightClass::GetSpotLight() {
	return this->spotLightProp;
}

bool LightClass::IsSpotLight() {
	return this->isSpotLight;
}

void LightClass::SetAmbientIntensity(glm::vec3 ambientIntensity) {
	LightClass::ambientIntensity = ambientIntensity;
}

void LightClass::SetPosition(glm::vec3 position) {
	this->pos = position;
}

void LightClass::Update(float time) {
	if (isSpotLight) {
		this->pos += this->speed;
	} else {
		this->pos += glm::vec3(
			this->speed.x,
			this->speed.y * time - g * time * time / 2,
			this->speed.z
		);

		this->speed.y = this->speed.y - g * time;
	}
}

/*
	Light Factory methods
*/

LightFactory* LightFactory::instance = 0;
glm::ivec2 LightFactory::spawnDist(0);

LightFactory::LightFactory() { }

LightFactory::~LightFactory() { }

void LightFactory::SetSpawnParams(glm::ivec2 dist) {
	this->spawnDist = dist;
}

LightFactory* LightFactory::GetInstance() {
	if (instance == NULL) {
		instance = new LightFactory();
	}
	return instance;
}

LightClass* LightFactory::SpawnLight(std::string color) {
	float time = 10.0f;
	glm::vec3 position(-1);
	glm::vec3 speed;

	if (rand() % 5 < 3) {
		position.z = -(rand() % (-spawnDist[1]));
		position.x = ((rand() % 2 == 0) ? -1.0 : 1.0) * (spawnDist[0] + rand() % (3 * spawnDist[0] + 1));

		speed = glm::vec3(
			-position.x / time * LightClass::scale,
			2 * (abs(position.x) + 1) / time + g * time / 4,
			0
		);

		if (color == "RED") return new LightClass(glm::vec3(2, 0, 0), position, speed, MaterialProperties(
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			100
		));
		if (color == "GREEN") return new LightClass(glm::vec3(0, 2, 0), position, speed, MaterialProperties(
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			100
		));
		if (color == "BLUE") return new LightClass(glm::vec3(0, 0, 2), position, speed, MaterialProperties(
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			100
		));
	} else {
		position = glm::vec3(Platform::rows[1], -0.5, spawnDist[1]);
		speed = glm::vec3(position.z / time * LightClass::scale * (rand() %2 == 0) ? 1: -1, 0, 0);

		return new LightClass(glm::vec3(100), position, speed, MaterialProperties(
			glm::vec3(0.1), glm::vec3(0.2), glm::vec3(0.5), glm::vec3(0.5), 10
		), spotlight(glm::vec3(0, 1, 0), 45));
	}


	return NULL;
}