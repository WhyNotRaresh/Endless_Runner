#pragma once

#include <include/glm.h>
#include <vector>
#include "../Platforms/Platform.h"

#define MAX_NUM_LIGHTS 20

namespace Light {
	const float g = 7;

	typedef struct spotlight {
		spotlight() :
			direction(0), angle(0) {}
		spotlight(glm::vec3 direction, float angle) :
			direction(direction), angle(angle) {}
		glm::vec3 direction;
		float angle;
	} spotlight;

	class LightClass {
	public:
		LightClass(glm::vec3 lightIntensity, glm::vec3 position);
		LightClass(glm::vec3 lightIntensity, glm::vec3 position, glm::vec3 speed, MaterialProperties material);
		LightClass(glm::vec3 lightIntensity, glm::vec3 position, glm::vec3 speed, MaterialProperties material, spotlight prop);
		~LightClass();

		static glm::vec3 GetAmbientIntensity();
		glm::vec3 GetLightIntensity();
		glm::vec3 GetPosition();
		glm::mat4 GetModelMatrix();
		MaterialProperties GetMaterial();
		spotlight GetSpotLight();
		bool IsSpotLight();
		static void SetAmbientIntensity(glm::vec3 ambientIntensity);
		void SetPosition(glm::vec3 position);
		void Update(float deltaTime);

		static float scale;
	private:
		static glm::vec3 ambientIntensity;
		glm::vec3 lightIntensity;
		bool isSpotLight;
		spotlight spotLightProp;
		MaterialProperties material;

		glm::vec3 speed;
		glm::vec3 pos;
	};


	class LightFactory {
	private:
		static LightFactory* instance;					// class instance
		LightFactory();

		static glm::ivec2 spawnDist;
	public:
		static LightFactory* GetInstance();				// returns instance of singleton factory class
		~LightFactory();

		void SetSpawnParams(glm::ivec2 dist);
		LightClass* SpawnLight(std::string color);
	};
}