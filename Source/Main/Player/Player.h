#pragma once

#include <include/glm.h>
#include <string>
#include "../Platforms/Platform.h"
#include "../Material.h"

class Player {
public:
	static Player* GetInstance();
	~Player();

	MaterialProperties GetMaterial();			// returns player's material struct
	glm::mat4 GetModelMatrix();					// returns player's model matirx
	std::string GetMeshName();					// returns mesh name for player's mesh
	glm::vec3 GetPosition();					// returns player's position

	void Update(float time, bool FAST_DOWN);	// updates player's position based on time passed
	void DashRight();							// dashes player to the right
	void DashLeft();							// dashes player to the left
	void Jump();								// makes player jump

	static void SetSpeedUp(int mod);			// sets player speedup manualy
	static int GetSpeedUp();					// returns player's speedup value
	void SetMaxSpeedUp();						// sets player into hyper speed for 2 second
	void IncreaseSpeed();						// increases player's speedup value
	void DecreaseSpeed();						// decreases player's speedup value

	static float GetFuel();						// returns player's fuel value [0:1]
	void DrainFuel();							// drains 20% of currnet player's fuel
	void AddFuel();								// increases player's fuel by 20%
	bool SetFatigued();							// sets player in fatigued state
	bool IsFatigued();							// returns player's fatigue state

	void IncScore();							// increases player's score (1 for each platform passed)
	int GetScore();								// returns player's score
	
	static float GetRadius();					// returns the sphear's radius
	void SetOnPlatform(float height);			// sets player on a platform @ height

	static Player* Reset();						// resets all player info
private:
	static Player* instance;
	Player();

	MaterialProperties playerMaterial;
	glm::mat4 modelMatrix;
	std::string meshName;
	glm::vec3 position;

	glm::vec3 speed;
	const float g = 7;
	float row;

	void NotifyFactory();		// notifies platform factory of speedup modification
	int speedUpModifier;		// index for player speedup [0:5]
	float maxSpeedUpTime;		// set to 2 second after being sent into hyperspeed
	bool fatigue;
	float fatigueTime;
	float fuel;
	int score;

	bool isOnPlatform;			// true if player is set on a platform. Changed by SetOnPlatform()
	float radius;
};