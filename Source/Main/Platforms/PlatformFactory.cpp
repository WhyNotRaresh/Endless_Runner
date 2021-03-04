#include "Platform.h"
#include "../Player/Player.h"

using namespace PlatformFactory;

PlatformFactoryClass* PlatformFactoryClass::instance = 0;
std::vector<glm::vec3> PlatformFactoryClass::spawningPoint = {
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0)
};

PlatformFactoryClass* PlatformFactoryClass::GetInstance() {
	if (instance == NULL) {
		instance = new PlatformFactoryClass();
	}
	return instance;
}

PlatformFactoryClass::PlatformFactoryClass() {
	propFactory = Prop::PropFactory::GetInstance();
	this->timeToRespawn = 0;
	this->timePassed = 0;
}

PlatformFactoryClass::~PlatformFactoryClass() {}

void PlatformFactoryClass::SetSpawningPoint(float spawningY, float spawningZ) {
	for (int i = 0; i < spawningPoint.size(); i++) {
		spawningPoint[i].x = Platform::rows[i];
		spawningPoint[i].y = spawningY;
		spawningPoint[i].z = spawningZ;
	}
}

Platform::BasicPlatform* PlatformFactoryClass::build(int row, std::string color) {
	if (row < 0 || row > 2) return NULL;

	glm::mat4 ModelMatrix = glm::scale(glm::translate(glm::mat4(1), this->spawningPoint[row]), glm::vec3(Platform::scale));
	Platform::BasicPlatform* platform;

	if (color == "RED")
		platform = new Platform::RedPlatform(
			row, ModelMatrix
		);

	else if (color == "YELLOW")
		platform = new Platform::YellowPlatform(
			row, ModelMatrix
		);
	else if (color == "ORANGE")
		platform = new Platform::OrangePlatform(
			row, ModelMatrix
		);

	else if (color == "GREEN")
		platform = new Platform::GreenPlatform(
			row, ModelMatrix
		);

	else
		platform =  new Platform::BasicPlatform(
			row, ModelMatrix
		);

	platform->SetOnPlatformProp(propFactory->SpawnRandomProp());
	return platform;
}

std::vector<Platform::BasicPlatform*> PlatformFactoryClass::SpawnPlatforms(std::unordered_map<std::string, glm::vec2> probabilities) {
	std::vector<Platform::BasicPlatform*> platformList;
	if (instance == NULL) return platformList;

	int goodRow = rand() % 3;
	float rng = (float)(rand() % 100) / 100;
	if (rng > Player::GetFuel()) {
		platformList.insert(platformList.end(), instance->build(goodRow, "GREEN"));
	}
	else {
		platformList.insert(platformList.end(), instance->build(goodRow, "PURPLE"));
	}

	std::string color;
	int colorRange = Platform::colorNames.size();
	for (int row = 0; row < 3; row++) {
		if (rand() % 100 <= 40 && row != goodRow) {
			rng = (float) (rand() % 100) / 100;
			std::string color;

			for (auto it : probabilities) {
				if (it.second.x <= rng && rng < it.second.y) {
					color = it.first;
					break;
				}
			}

			platformList.push_back(instance->build(row, color));
		}
	}

	return platformList;
}

std::vector<Platform::BasicPlatform*> PlatformFactoryClass::InitPlatformList(std::unordered_map<std::string, glm::vec2> initProbabilities) {
	Player::SetSpeedUp(1);
	int cycles = (int) (-spawningPoint[0].z / (Platform::speed[1].z * Platform::scale.z * timeToRespawn));

	std::vector<Platform::BasicPlatform*> list;
	list.insert(list.end(), instance->build(1, ""));
	list[0]->Update(timeToRespawn);
	list.insert(list.end(), instance->build(1, ""));
	for (int i = 0; i < cycles - 1; i++) {
		for (int j = 0; j < list.size(); j++) {
			list[j]->Update(timeToRespawn);
		}

		std::vector<Platform::BasicPlatform*> newPlatforms = instance->SpawnPlatforms(initProbabilities);
		list.insert(list.end(), newPlatforms.begin(), newPlatforms.end());
	}

	list[0]->SetOnPlatformProp(NULL);
	list[1]->SetOnPlatformProp(NULL);
	Player::SetSpeedUp(0);
	return list;
}

bool PlatformFactoryClass::AreRowsFree(std::vector<Platform::BasicPlatform*> lastThreePlatforms) {
	for (Platform::BasicPlatform* platform: lastThreePlatforms) {
		if (abs(spawningPoint[0].z - platform->GetPosition().z) < Platform::platformLength * Platform::scale.z) {
			return false;
		}
	}
	return true;
}

void PlatformFactoryClass::UpdateRespawnTime() {
	if (instance != NULL) {
		instance->timeToRespawn = Platform::platformLength / Platform::speed[Player::GetSpeedUp()].z;
	}
}