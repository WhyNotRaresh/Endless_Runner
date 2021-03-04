#pragma once

#include <include/glm.h>
#include "../../Core/GPU/Texture2D.h"
#include <unordered_map>

// All loaded textures
class Textures {
public:
	static std::unordered_map<std::string, Texture2D*> map;
};

/*
	Struct that contains material information for all objects in scene
*/

typedef struct material {
	material() :
		materialKe(glm::vec3(0, 0.7, 0)), materialKa(glm::vec3(0.25)), materialKd(glm::vec3(0.5)), materialKs(glm::vec3(0.5)), materialShininess(10), texture(NULL) {}
	material(glm::vec3 Ke, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, int shinniness)
		: materialKe(Ke), materialKa(Ka), materialKd(Kd), materialKs(Ks), materialShininess(shinniness), texture(NULL) {}
	material(glm::vec3 Ke, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, int shinniness, Texture2D* tex)
		: materialKe(Ke), materialKa(Ka), materialKd(Kd), materialKs(Ks), materialShininess(shinniness), texture(tex) {}
	glm::vec3 materialKe,
		materialKa,
		materialKd,
		materialKs;
	int materialShininess;
	Texture2D* texture;
} MaterialProperties;