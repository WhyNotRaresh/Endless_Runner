#include "Tema2.h"

#include <Core/Engine.h>
#include <include/math.h>
#include <vector>
#include <string>

#include <fstream>
#include <iostream>

using namespace std;

Tema2::Tema2(){ }

Tema2::~Tema2(){ }

void Tema2::PrintTextToConsole() {
	printf("=======================================================\n");
	printf("========================WELCOME========================\n");
	printf("=======================================================\n\n");
	printf("Cortrol scheme is :\n");
	printf("\t->Space for jump;\n");
	printf("\t->Up, Down for speed control;\n");
	printf("\t->Left, Right for lane change;\n");
	printf("\t->S for going down faster;\n");
	printf("\t->P to change to 1st/3rd person.\n");
	printf("The amount of fuel left is denoted by the background mountains left uncovered.\n\n");
	printf("Press Up to start the game(set player to 1st speed)\n");
	printf("========================HAVEFUN========================\n");
}

std::unordered_map<std::string, Texture2D*> Textures::map = {};
void Tema2::Init()
{
	//PlaySound(TEXT("..\\Source\\Laboratoare\\Tema2\\m.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT); 

	totalTime = 0;
	GameInProgress = true;

	camera = new Camera::CameraClass(glm::vec3(0, 15, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 200.0f, RADIANS(-20));
	camera->SetProjectionMatrix(glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, camera->GetRenderDistance()));
	LoadProgramMeshes();
	LoadProgramTextures();

	player = player->GetInstance();

	factoryP = factoryP->GetInstance();
	factoryP->SetSpawningPoint(10, -camera->GetRenderDistance() * 0.7);
	probabilities = {
		{"GREEN", glm::vec2(0, 0)},
		{"YELLOW", glm::vec2(0, 0.4)},
		{"ORANGE", glm::vec2(0.4, 0.6)},
		{"PURPLE", glm::vec2(0.6, 1)},
		{"RED", glm::vec2(1, 1)},
	};
	platformList = factoryP->InitPlatformList(probabilities);
	despawnLine = 10;

	/*
		Setting light properties
	*/
	Light::LightClass::SetAmbientIntensity(glm::vec3(0.1));
	mainLight = new Light::LightClass(glm::vec3(0.5), glm::vec3(0, 20, -10));
	factoryL = factoryL->GetInstance();
	factoryL->SetSpawnParams(glm::vec2(20, -camera->GetRenderDistance() * 0.7));

	flags = (GLint*)calloc(4, sizeof(GLint));

	PrintTextToConsole();
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	totalTime += deltaTimeSeconds;
	RenderMeshes(deltaTimeSeconds);
	if (!GameInProgress) return;

	/*
		Player Update
	*/
	{
		// Informing player of time passage
		player->Update(deltaTimeSeconds, window->KeyHold(GLFW_KEY_S));

		player->SetOnPlatform(RelativePlayerPosition());

		// Checking if player still has fuel or player is under y = 5
		if (player->GetFuel() <= 0 || player->GetPosition().y <= 5) {
			EndGame();
		}

		if (window->KeyHold(GLFW_KEY_SPACE)) {
			player->Jump();
		}

		if (window->KeyHold(GLFW_KEY_RIGHT)) {
			player->DashRight();
		}

		if (window->KeyHold(GLFW_KEY_LEFT)) {
			player->DashLeft();
		}
	}

	/*
		Platforms Update
	*/
	{
		// Updating platform positions and Removing platforms after despawn line
		for (int i = 0; i < platformList.size(); i++) {
			platformList[i]->Update(deltaTimeSeconds);

			if (platformList[i]->GetPosition().z > despawnLine) {
				platformList.erase(platformList.begin() + i, platformList.begin() + i + 1);
				i--;
			}
		}

		// Platform spawninng
		if (factoryP->timePassed + deltaTimeSeconds >= factoryP->timeToRespawn &&
				factoryP->AreRowsFree(std::vector<Platform::BasicPlatform*>(platformList.end() - 3, platformList.end()))) {
			std::vector<Platform::BasicPlatform*> newPlatforms = factoryP->SpawnPlatforms(probabilities);
			platformList.insert(platformList.end(), newPlatforms.begin(), newPlatforms.end());
			factoryP->timePassed = 0;
			player->IncScore();
		}
		else {
			if (player->GetSpeedUp() != 0) factoryP->timePassed += deltaTimeSeconds;
		}

		// Updating spawning probabilities
		UpdateProbabilities();
	}

	/*
		Lights Update
	*/
	{
		if (rand() % 100 < 1 && auxLights.size() < MAX_NUM_LIGHTS) {
			int color = rand() % 3;
			switch (color) {
			case 0: auxLights.push_back(factoryL->SpawnLight("RED")); break;
			case 1: auxLights.push_back(factoryL->SpawnLight("BLUE")); break;
			case 2: auxLights.push_back(factoryL->SpawnLight("GREEN")); break;
			}
		}

		for (int i = 0; i < auxLights.size(); i++) {
			auxLights[i]->Update(deltaTimeSeconds);

			if (auxLights[i]->GetPosition().y < -2) {
				auxLights.erase(auxLights.begin() + i, auxLights.begin() + i + 1);
				i--;
			}
		}
	}
}

void Tema2::FrameEnd() { }

float Tema2::RelativePlayerPosition() {
	if (!GameInProgress) return -1;

	glm::vec3 platformDim = glm::vec3(Platform::platformWidth, Platform::platformThickness, Platform::platformLength) * Platform::scale;		// relative platform dimensions

	for (int i = 0; i < platformList.size(); i++) {
		Prop::OnPlatformProp* prop = platformList[i]->GetOnPLatformProp();
		if (prop != NULL) {
			glm::vec3 pos = prop->GetCollisionPoint(platformList[i]->GetPosition());

			if (abs(glm::distance(pos, player->GetPosition())) < player->GetRadius()) {
				if (prop->IsHard())
					if (player->SetFatigued()) {
						EndGame(); 
						return -1;
					}
				else
					player->IncScore();
				platformList[i]->SetOnPlatformProp(NULL);
			}
		}

		glm::vec3 delta = glm::abs(player->GetPosition() - platformList[i]->GetPosition());
		if (delta.x <= platformDim.x / 2 && delta.y <= player->GetRadius() && delta.z <= platformDim.z / 2) {
			int effectCode = platformList[i]->GetEffect();

			switch (effectCode) {
			case 0: {
				EndGame();
				break;
			}
			case 1: {
				player->DrainFuel();
				break;
			}
			case 2: {
				player->SetMaxSpeedUp();
				break;
			}
			case 3: {
				player->AddFuel();
				break;
			}
			default:
				break;
			}

			platformList[i] = platformList[i]->MakeBasic();
			return platformList[i]->GetPosition().y;
		}
	}
	return -1;
}

void Tema2::UpdateProbabilities() {
	// Spawning more green platforms as fuel gets lower
	probabilities["GREEN"].y = (1 - player->GetFuel()) * 0.3;
	probabilities["YELLOW"].x = probabilities["GREEN"].y;

	// Spawning more red platforms as time goes on
	probabilities["PURPLE"].y = 1 - (100 - std::min(100, player->GetScore() / 2)) / 100 * 0.2;
	probabilities["RED"].x = probabilities["PURPLE"].y;
}

void Tema2::RenderMeshes(float deltaTimeSeconds)
{
	/*
		Rendering player
	*/
	{
		flags[0] = 0; flags[1] = 0; flags[2] = 0; flags[3] = 0;
		if (!camera->IsFirstPerson())
			RenderMesh(meshes[player->GetMeshName()], shaders["SimpleShader"], player->GetModelMatrix(), player->GetMaterial());

		if (player->IsFatigued()) {
			flags[0] = 1; flags[1] = 0; flags[2] = 0; flags[3] = 1;
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1), camera->GetPosition() + glm::vec3(0, 0, -0.5));
			if (!camera->IsFirstPerson()) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5 * tan(camera->GetAngleOXRotation()), 0));
				modelMatrix = glm::rotate(modelMatrix, camera->GetAngleOXRotation(), glm::vec3(1, 0, 0));
			}
			modelMatrix = glm::scale(modelMatrix, glm::vec3(2.6, 0.9, 1));
			RenderMesh(meshes["square"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
				glm::vec3(0.1),
				glm::vec3(0.1),
				glm::vec3(0.1),
				glm::vec3(0.1),
				25,
				Textures::map["FATIGUE"]
			));
		}
	}

	/*
		Rendering Platforms
	*/
	{
		for (int i = 0; i < platformList.size(); i++) {
			flags[0] = 1; flags[1] = 0; flags[2] = 0; flags[3] = 0;
			Platform::BasicPlatform* p = platformList[i];
			RenderMesh(meshes[p->GetMeshName()], shaders["SimpleShader"], p->GetModelMatrix(), p->GetMaterial());

			Prop::OnPlatformProp* prop = p->GetOnPLatformProp();
			if (prop != NULL) {
				flags[0] = 1; flags[1] = 1; flags[2] = 0; flags[3] = 0;
				MaterialProperties material(glm::vec3(0.5), glm::vec3(0.3), glm::vec3(0.7), glm::vec3(0.7), 30);
				if (prop->IsHard()) {
					material.texture = Textures::map["CRAZY_NEON"];
				} else {
					material.texture = Textures::map["NEON_DARK_BLUE"];
				}

				RenderMesh(meshes[prop->GetMeshName()], shaders["SimpleShader"], prop->ComputeModelMatrix(p->GetPosition()), material);
			}
		}
	}

	/*
		Render stage
	*/
	{
		flags[0] = 1; flags[1] = 0; flags[2] = 0; flags[3] = 0;
		// Stage
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, -camera->GetRenderDistance()));
		RenderMesh(meshes["stage"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
			glm::vec3(0),
			glm::vec3(0.1),
			glm::vec3(0.2),
			glm::vec3(0.2),
			100,
			Textures::map["GROUND"]
		));

		flags[0] = 0; flags[1] = 0; flags[2] = 0;
		// Plane
		modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, -0.1, -camera->GetRenderDistance() / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2 * camera->GetRenderDistance()));
		RenderMesh(meshes["plane"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
			glm::vec3(0.01),
			glm::vec3(0.3),
			glm::vec3(0.15),
			glm::vec3(0.5),
			50
		));

		flags[0] = 0; flags[1] = 0; flags[2] = 0;
		// Left cilinder
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, (float)std::atan(-30 / camera->GetRenderDistance()), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, -2 * camera->GetRenderDistance()));
		RenderMesh(meshes["cilinder"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
			glm::vec3(0.5, 0, 0.5),
			glm::vec3(0.2),
			glm::vec3(0.7),
			glm::vec3(0.7),
			30
		));

		// Rignt cilinder
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(30, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, (float)std::atan(30 / camera->GetRenderDistance()), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, -2 * camera->GetRenderDistance()));
		RenderMesh(meshes["cilinder"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
			glm::vec3(0.5, 0, 0.5),
			glm::vec3(0.2),
			glm::vec3(0.7),
			glm::vec3(0.7),
			30
		));
	}

	/*
		Rendering Mountains
	*/
	{
		flags[0] = 1; flags[1] = 1; flags[2] = 1; flags[3] = 0;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -camera->GetRenderDistance() * 0.7));
		RenderMesh(meshes["mountains"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
			glm::vec3(0.1),
			glm::vec3(0.1),
			glm::vec3(0.7),
			glm::vec3(0.7),
			150,
			Textures::map["NEON_LIGHT_BLUE"]
		));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -player->GetFuel() * 50, 4));
		RenderMesh(meshes["mountains_2"], shaders["SimpleShader"], modelMatrix, MaterialProperties(
			glm::vec3(0.05),
			glm::vec3(0.1),
			glm::vec3(0.6),
			glm::vec3(0.6),
			150,
			Textures::map["NEON_YELLOW"]
		));
	}

	/*
		Rendering extra lights
	*/
	{
		flags[0] = 0; flags[1] = 0; flags[2] = 0; flags[3] = 0;
		for (int i = 0; i < auxLights.size(); i++) {
			RenderMesh(meshes["sphere"], shaders["SimpleShader"], auxLights[i]->GetModelMatrix(), auxLights[i]->GetMaterial());
		}
	}
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, MaterialProperties material)
{
	if (!mesh || !shader || !shader->program)
		return;
	glUseProgram(shader->program);
	int location;

	// Set shader uniforms for light position and proprieties
	location = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(location, 1, glm::value_ptr(mainLight->GetPosition()));

	location = glGetUniformLocation(shader->program, "intensity_ambient");
	glUniform3fv(location, 1, glm::value_ptr(Light::LightClass::GetAmbientIntensity()));

	location = glGetUniformLocation(shader->program, "intensity_light");
	glUniform3fv(location, 1, glm::value_ptr(mainLight->GetLightIntensity()));


	// Set shade uniforms for Auxiliary Lights
	glm::vec3 position[MAX_NUM_LIGHTS], intensity[MAX_NUM_LIGHTS], direction[MAX_NUM_LIGHTS];
	int isSpotlight[MAX_NUM_LIGHTS];
	float angle[MAX_NUM_LIGHTS];
	int count = 0;
	for (int i = 0 ; i < auxLights.size(); i++) {
		if (auxLights[i]->GetPosition().y > -1) {
			position[count] = auxLights[i]->GetPosition();
			intensity[count] = auxLights[i]->GetLightIntensity();

			if (auxLights[i]->IsSpotLight()) {
				isSpotlight[count] = 1;
				direction[count] = auxLights[i]->GetSpotLight().direction;
				angle[count] = auxLights[i]->GetSpotLight().angle;
			} else {
				isSpotlight[count] = 0;
			}

			count++;
		}
	}

	location = glGetUniformLocation(shader->program, "aux_count");
	glUniform1i(location, count);

	location = glGetUniformLocation(shader->program, "aux_lights_positions");
	glUniform3fv(location, MAX_NUM_LIGHTS, glm::value_ptr(position[0]));

	location = glGetUniformLocation(shader->program, "aux_lights_intensity");
	glUniform3fv(location, MAX_NUM_LIGHTS, glm::value_ptr(intensity[0]));

	location = glGetUniformLocation(shader->program, "aux_lights_isSpotlight");
	glUniform1iv(location, MAX_NUM_LIGHTS, isSpotlight);

	location = glGetUniformLocation(shader->program, "aux_lights_directions");
	glUniform3fv(location, MAX_NUM_LIGHTS, glm::value_ptr(direction[0]));

	location = glGetUniformLocation(shader->program, "aux_lights_angles");
	glUniform1fv(location, MAX_NUM_LIGHTS, angle);

	// Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->GetPosition();
	location = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));

	// Set material property uniforms (shininess, ka, ke, kd, ks)
	location = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(location, material.materialShininess);

	location = glGetUniformLocation(shader->program, "material_ka");
	glUniform3f(location, material.materialKa.x, material.materialKa.y, material.materialKa.z);

	location = glGetUniformLocation(shader->program, "material_ke");
	glUniform3f(location, material.materialKe.x, material.materialKe.y, material.materialKe.z);

	location = glGetUniformLocation(shader->program, "material_kd");
	glUniform3f(location, material.materialKd.x, material.materialKd.y, material.materialKd.z);

	location = glGetUniformLocation(shader->program, "material_ks");
	glUniform3f(location, material.materialKs.x, material.materialKs.y, material.materialKs.z);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Set time
	location = glGetUniformLocation(shader->program, "time_elapsed");
	glUniform1f(location, totalTime);

	// Send texture info
	location = glGetUniformLocation(shader->program, "fragment_flags");
	glUniform1iv(location, 3, flags);

	location = glGetUniformLocation(shader->program, "vertex_flag");
	glUniform1i(location, flags[3]);

	if (material.texture != NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::EndGame() {
	if (GameInProgress) {
		std::cout << "----------Game Finished----------" << std::endl;
		std::cout << "Your Score:\t" << player->GetScore() << std::endl;
		std::cout << "Press up arrow to start new game." << std::endl;
		std::cout << "---------------------------------" << std::endl;
		player->SetSpeedUp(0);
		GameInProgress = false;
	}
}

void Tema2::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_UP) {
		player->IncreaseSpeed();
		if (!GameInProgress) {
			probabilities = {
				{"GREEN", glm::vec2(0, 0)},
				{"YELLOW", glm::vec2(0, 0.4)},
				{"ORANGE", glm::vec2(0.4, 0.6)},
				{"PURPLE", glm::vec2(0.6, 1)},
				{"RED", glm::vec2(1, 1)},
			};
			platformList = factoryP->InitPlatformList(probabilities);
			player = Player::Reset();
			GameInProgress = true;
		}
	}

	if (key == GLFW_KEY_DOWN) {
		player->DecreaseSpeed();
	}

	if (key == GLFW_KEY_P) {
		camera->ChangePerson();
	}
}

void Tema2::OnInputUpdate(float deltaTime, int mods){ }

void Tema2::OnKeyRelease(int key, int mods){ }

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){ }

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods){ }

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods){ }

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY){ }

void Tema2::OnWindowResize(int width, int height){ }