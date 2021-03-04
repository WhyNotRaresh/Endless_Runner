#pragma once

#include <vector>
#include <include/glm.h>

namespace Prop {
	class OnPlatformProp {
	public:
		OnPlatformProp(float position, bool hardness, std::string meshName);
		~OnPlatformProp();

		glm::vec3 GetCollisionPoint(glm::vec3 platformPosition);
		std::string GetMeshName();
		bool IsHard();

		glm::mat4 ComputeModelMatrix(glm::vec3 platformPosition);
	private:
		float position;
		std::string meshName;
		bool hard;
	};

	class PropFactory {
	private:
		static PropFactory* instance;					// class instance
		PropFactory();
	public:
		static PropFactory* GetInstance();				// returns instance of singleton factory class
		~PropFactory();

		OnPlatformProp* SpawnRandomProp();
	};
}