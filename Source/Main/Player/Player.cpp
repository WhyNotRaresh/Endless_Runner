#include "Player.h"
#include <math.h>

Player* Player::instance = 0;

Player* Player::GetInstance() {
	if (instance == NULL) {
		instance = new Player();
	}
	return instance;
}

Player::Player() {
	this->meshName = "sphere";
	this->playerMaterial = MaterialProperties(
		glm::vec3(0.1),
		glm::vec3(0.1, 0.3, 0.5),
		glm::vec3(0.1, 0.6, 0.7),
		glm::vec3(0.1, 0.6, 0.7),
		25
	);
	this->position = glm::vec3(0, 12, -10);
	this->speed = glm::vec3(0);
	float scale = 0.05;
	this->modelMatrix = glm::scale(glm::translate(glm::mat4(1), this->position), glm::vec3(scale));
	this->radius = 20 * scale;
	this->row = 1;

	this->fuel = 1;
	this->speedUpModifier = 0;
	this->maxSpeedUpTime = 0;
	this->fatigue = false;
	this->fatigueTime = 0;

	this->score = 0;
	this->isOnPlatform = true;
}

Player::~Player() {}

void Player::Update(float time, bool FAST_DOWN) {
	int speedUp = 1;
	if (FAST_DOWN) speedUp = 10;

	if (isOnPlatform) {
		this->position += glm::vec3(
			this->speed.x,
			0,
			this->speed.z
		);
	}
	else {
		this->position += glm::vec3(
			this->speed.x,
			this->speed.y * time - speedUp * g * time * time / 2,
			this->speed.z
		);
	}

	this->speed = glm::vec3(
		this->speed.x + ((this->speed.x < 0) ? 1 : -1) * time,
		this->speed.y - speedUp * g * time,
		this->speed.z
	);

	if (std::roundf(this->row) != this->row) {
		if (this->position.x <= Platform::rows[this->row - 0.5]) {
			this->row -= 0.5;
			this->position.x = Platform::rows[this->row];
			this->speed.x = 0;
		}
		else if (this->position.x >= Platform::rows[this->row + 0.5]) {
			this->row += 0.5;
			this->position.x = Platform::rows[this->row];
			this->speed.x = 0;
		}
	}
	else {
		this->speed.x = 0;
	}

	this->modelMatrix = glm::scale(glm::translate(glm::mat4(1), this->position), glm::vec3(0.05));

	this->fuel -= time / 80 * (speedUpModifier / 2.0);
	if (maxSpeedUpTime != 0) {
		maxSpeedUpTime -= time;
		if (maxSpeedUpTime <= 0) {
			maxSpeedUpTime = 0;
			DecreaseSpeed();
		}
	}

	if (fatigueTime != 0) {
		fatigueTime -= time;
		if (fatigueTime <= 0) {
			fatigueTime == 0;
			fatigue = false;
		}
	}
}

void Player::DashRight() {
	if (std::roundf(this->row) == this->row && this->row != 2) {
		this->speed.x = 0.4;
		this->row += 0.5;
	}
}

void Player::DashLeft() {
	if (std::roundf(this->row) == this->row && this->row != 0) {
		this->speed.x = -0.4;
		this->row -= 0.5;
	}
}

void Player::Jump() {
	if (isOnPlatform) {
		this->speed.y = 6;
		isOnPlatform = false;
	}
}

MaterialProperties Player::GetMaterial() {
	return this->playerMaterial;
}

glm::mat4 Player::GetModelMatrix() {
	return this->modelMatrix;
}

std::string Player::GetMeshName() {
	return this->meshName;
}

glm::vec3 Player::GetPosition() {
	return this->position;
}

void Player::SetSpeedUp(int mod) {
	if (instance != NULL) {
		instance->speedUpModifier = mod;
		instance->NotifyFactory();
	}
}

int Player::GetSpeedUp() {
	if (instance != NULL)
		return instance->speedUpModifier;
	else
		return 0;
}

void Player::IncreaseSpeed() {
	if (speedUpModifier < Platform::speed.size() - 2) {
		speedUpModifier++;
		NotifyFactory();
	}
}

void Player::DecreaseSpeed() {
	if (speedUpModifier != 1) {
		speedUpModifier--;
		NotifyFactory();
	}
}

void Player::SetMaxSpeedUp() {
	this->speedUpModifier = Platform::speed.size() - 1;
	this->maxSpeedUpTime = 2;
	NotifyFactory();
}

bool Player::SetFatigued() {
	if (fatigue == true) {
		return true;
	} else {
		fatigue = true;
		fatigueTime = 7;
	}
	return false;
}

bool Player::IsFatigued() {
	return this->fatigue;
}

void Player::NotifyFactory() {
	PlatformFactory::PlatformFactoryClass::UpdateRespawnTime();
}

float Player::GetFuel() {
	if (instance != NULL)
		return instance->fuel;
	else
		return 0;
}

void Player::DrainFuel() {
	this->fuel -= 0.2 * this->fuel;
}

void Player::AddFuel() {
	this->fuel += 0.2 * this->fuel;
	if (this->fuel > 1) {
		this->fuel = 1;
	}
}

void Player::IncScore() {
	this->score++;
}

int Player::GetScore() {
	return this->score;
}

float Player::GetRadius() {
	if (instance != NULL)
		return instance->radius;
	else
		return 0;
}

void Player::SetOnPlatform(float height) {
	if (height != -1) {
		this->position.y = height + this->radius;
		this->isOnPlatform = true;
	}
	else {
		this->isOnPlatform = false;
	}
}

Player* Player::Reset() {
	instance = NULL;
	return Player::GetInstance();
}