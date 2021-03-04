#pragma once

#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Camera.h"
#include "Material.h"
#include "Platforms/Platform.h"
#include "Player/Player.h"
#include "Lights/Light.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void LoadProgramMeshes();
	void LoadProgramTextures();
	void RenderMeshes(float deltaTimeSeconds);
	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, MaterialProperties material);

	void UpdateProbabilities();
	float RelativePlayerPosition();

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void PrintTextToConsole();
	void EndGame();
protected:
	Camera::CameraClass* camera;

	float totalTime;

	Light::LightClass* mainLight;
	std::vector<Light::LightClass*> auxLights;
	Light::LightFactory* factoryL;

	/*
		Flags to be sent to shade
		[0] : has texture
		[1] : loop texture horizontally
		[2] : loop texture vertically
	*/
	GLint* flags;

	PlatformFactory::PlatformFactoryClass* factoryP;
	std::vector<Platform::BasicPlatform*> platformList;
	std::unordered_map<std::string, glm::vec2> probabilities;
	float despawnLine;

	Player* player;
	bool GameInProgress;
};
