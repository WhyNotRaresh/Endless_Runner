#pragma once

#include <include/glm.h>
#include <include/math.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Material.h"
#include "Prop.h"


/*
	File inculdes all definitions for all platforms
*/

namespace Platform {
	const glm::vec3 scale = glm::vec3(0.1f);				// scale factor for mesh
	const float platformLength = 120;						// platform length as per .obj file
	const float platformThickness = 3;						// platform thickness as per .obj file
	const float platformWidth = 25;							// platform width as per .obj file

	const std::vector<glm::vec3> speed = {					// possible speeds for platforms
		glm::vec3(0, 0.005, 0),			// static
		glm::vec3(0, 0.005, 100),
		glm::vec3(0, 0.005, 140),
		glm::vec3(0, 0.005, 180),
		glm::vec3(0, 0.005, 300),		// hyper speed
	};
	const std::vector<float> rows = { -4.5, 0, 4.5 };		// predifined x positions for platform spawning

	const std::unordered_map<std::string, glm::vec3> predifinedColors = {		// predifined platform color values
			{"PURPLE", glm::vec3(0.5, 0, 0.5)},
			{"RED", glm::vec3(0.7, 0, 0)},
			{"YELLOW", glm::vec3(0.7, 0.7, 0)},
			{"ORANGE", glm::vec3(0.7, 0.5, 0)},
			{"GREEN", glm::vec3(0, 0.7, 0)}
	};

	const std::vector<std::string> colorNames = {			// predifined color names
		"PURPLE", "RED", "YELLOW", "ORANGE", "GREEN"
	};

	class BasicPlatform {
	public:
		BasicPlatform(int row, glm::mat4 ModelMatrix);
		~BasicPlatform();

		BasicPlatform* MakeBasic();						// transforms any platform in a basic one

		void Update(float deltaTime);					// updates position of platform based on playing speed
		glm::mat4 GetModelMatrix();						// returns platform's model matrix
		glm::vec3 GetPosition();						// returns platform's positon
		glm::vec3 GetColor();							// returns platform's color as a vec3
		MaterialProperties GetMaterial();				// returns platform's material composition
		std::string GetMeshName();						// returns mesh name for platform
		std::string GetColorName();						// returns name of platform's color
		int GetRow();									// returns platform's row index
		virtual int GetEffect();						// returns platform's effect; different for each inheriting class

		void SetOnPlatformProp(Prop::OnPlatformProp* prop);
		Prop::OnPlatformProp* GetOnPLatformProp();
	private:
		MaterialProperties platformMaterial;
		glm::mat4 ModelMatrix;
		std::string color;
		glm::vec3 position;
		int row;
		Prop::OnPlatformProp* prop;

	protected:
		BasicPlatform(int row, std::string color, MaterialProperties mp, glm::mat4 ModelMatrix);	// platform constructor only accessible by inheriting classes
	};

	/*
		Inheriting Classes for BasicPlatform
	*/
	class RedPlatform : public BasicPlatform {
	public:
		RedPlatform(int row, glm::mat4 ModelMatrix);
		~RedPlatform();

		int GetEffect() override;
	};

	class YellowPlatform : public BasicPlatform {
	public:
		YellowPlatform(int row, glm::mat4 ModelMatrix);
		~YellowPlatform();

		int GetEffect() override;
	};

	class OrangePlatform : public BasicPlatform {
	public:
		OrangePlatform(int row, glm::mat4 ModelMatrix);
		~OrangePlatform();

		int GetEffect() override;
	};

	class GreenPlatform : public BasicPlatform {
	public:
		GreenPlatform(int row, glm::mat4 ModelMatrix);
		~GreenPlatform();

		int GetEffect() override;
	};
}

/*
	Singleton factory class for creating platforms
*/
namespace PlatformFactory {
	class PlatformFactoryClass {
	private:
		static PlatformFactoryClass* instance;								// class instance
		PlatformFactoryClass();

		Prop::PropFactory* propFactory;
		static std::vector<glm::vec3> spawningPoint;						// spawning points for the 3 predifined rows
		Platform::BasicPlatform* build(int row, std::string color);			// build platform
	public:
		static PlatformFactoryClass* GetInstance();		// returns instance of singleton factory class
		~PlatformFactoryClass();

		static void UpdateRespawnTime();				// sets spawning time a new value based on plaing speed
		float timeToRespawn;							// time until SpawnPlatforms should be recalled
		float timePassed;								// time passed until last call of SpawnPlatforms

		void SetSpawningPoint(float spawningY, float spawningZ);																	// sets spawn point for future platforms
		bool AreRowsFree(std::vector<Platform::BasicPlatform*> lastThreePlatforms);													// checks if spawning point is free of platforms
		std::vector<Platform::BasicPlatform*> InitPlatformList(std::unordered_map<std::string, glm::vec2> initProbabilities);		// initiates platform list in Tema2.cpp
		std::vector<Platform::BasicPlatform*> SpawnPlatforms(std::unordered_map<std::string, glm::vec2> prob);						// Spawns platforms if timePassed > timeToRespawn
	};
}