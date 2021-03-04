#include "Platform.h"
#include "../Player/Player.h"

using namespace Platform;

BasicPlatform::BasicPlatform(int row, glm::mat4 ModelMatrix) {
	this->row = row;
	this->color = "PURPLE";
	this->position = glm::vec3(ModelMatrix[3]);
	this->ModelMatrix = ModelMatrix;
	this->platformMaterial = MaterialProperties(GetColor(), glm::vec3(0.2), glm::vec3(0.7), glm::vec3(0.7), 30, Textures::map["PURPLE"]);
}

BasicPlatform* BasicPlatform::MakeBasic() {
	BasicPlatform* platform = new BasicPlatform(this->row, this->ModelMatrix);
	platform->SetOnPlatformProp(this->GetOnPLatformProp());
	return platform;
}

void BasicPlatform::Update(float deltaTime) {
	glm::vec3 deltaLength = speed[Player::GetSpeedUp()] * deltaTime;
	ModelMatrix = glm::translate(ModelMatrix, deltaLength);
	this->position += deltaLength * Platform::scale;
}

glm::mat4 BasicPlatform::GetModelMatrix() {
	return this->ModelMatrix;
}

glm::vec3 BasicPlatform::GetPosition() {
	return this->position;
}

std::string BasicPlatform::GetMeshName() {
	return "platform";
}

glm::vec3 BasicPlatform::GetColor() {
	auto search = predifinedColors.find(this->color);
	if (search != predifinedColors.end())
		return search->second;
	else
		return glm::vec3(0);
}

std::string BasicPlatform::GetColorName() {
	return this->color;
}

int BasicPlatform::GetRow() {
	return this->row;
}

MaterialProperties BasicPlatform::GetMaterial() {
	return this->platformMaterial;
}

void BasicPlatform::SetOnPlatformProp(Prop::OnPlatformProp* prop) {
	this->prop = prop;
}

Prop::OnPlatformProp* BasicPlatform::GetOnPLatformProp() {
	return this->prop;
}


/*
	Constructors for all non-basic platfroms
*/

RedPlatform::RedPlatform(int row, glm::mat4 ModelMatrix)
	: BasicPlatform(row, "RED", MaterialProperties(glm::vec3(0), glm::vec3(0.4), glm::vec3(1, 0.2, 0.2), glm::vec3(1, 0.2, 0.2), 70, Textures::map["RED"]), ModelMatrix) {}

YellowPlatform::YellowPlatform(int row, glm::mat4 ModelMatrix)
	: BasicPlatform(row, "YELLOW", MaterialProperties(glm::vec3(0), glm::vec3(0.4), glm::vec3(0.8, 0.8, 0.2), glm::vec3(0.8, 0.8, 0.2), 70, Textures::map["YELLOW"]), ModelMatrix) {}

OrangePlatform::OrangePlatform(int row, glm::mat4 ModelMatrix)
	: BasicPlatform(row, "ORANGE", MaterialProperties(glm::vec3(0), glm::vec3(0.4), glm::vec3(0.7, 0.5, 0.2), glm::vec3(0.7, 0.5, 0.2), 70, Textures::map["ORANGE"]), ModelMatrix) {}

GreenPlatform::GreenPlatform(int row, glm::mat4 ModelMatrix)
	: BasicPlatform(row, "GREEN", MaterialProperties(glm::vec3(0), glm::vec3(0.4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), 70, Textures::map["GREEN"]), ModelMatrix) {}

BasicPlatform::BasicPlatform(int row, std::string color, MaterialProperties mp, glm::mat4 ModelMatrix) {
	this->row = row;
	this->color = color;
	this->position = glm::vec3(ModelMatrix[3]);
	this->ModelMatrix = ModelMatrix;
	this->platformMaterial = mp;
	this->platformMaterial.materialKe = GetColor();
}

/*
	Platform destructors
*/

BasicPlatform::~BasicPlatform() {}

RedPlatform::~RedPlatform() {}

YellowPlatform::~YellowPlatform() {}

OrangePlatform::~OrangePlatform() {}

GreenPlatform::~GreenPlatform() {}

/*
	Effects for platforms
*/

int BasicPlatform::GetEffect() {
	return -1;
}

int RedPlatform::GetEffect() {
	return 0;
}

int YellowPlatform::GetEffect() {
	return 1;
}

int OrangePlatform::GetEffect() {
	return 2;
}

int GreenPlatform::GetEffect() {
	return 3;
}