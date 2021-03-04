#include "Tema2.h"
#include "BasicShapes.h"

void Tema2::LoadProgramMeshes() {

	// Platform
	{
		Mesh* mesh = new Mesh("platform");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "Platform.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Stage
	{
		Mesh* mesh = new Mesh("stage");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "Stage.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	
	// Cilinder object
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		createCilinder(&vertices, &indices);

		Mesh* mesh = new Mesh("cilinder");
		mesh->InitFromData(vertices, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Pyramid object
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		createPyramid(&vertices, &indices);

		Mesh* mesh = new Mesh("pyramid");
		mesh->InitFromData(vertices, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Square object
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		createSquare(&vertices, &indices);

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Player
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "Player.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Mountains
	{
		Mesh* mesh = new Mesh("mountains");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "Mountains.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* mesh2 = new Mesh("mountains_2");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS, "Mountains2.obj");
		meshes[mesh2->GetMeshID()] = mesh2;
	}

	// Plane below stage
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		createSquare(&vertices, &indices);

		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
}


void Tema2::LoadProgramTextures() {
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Yellow.jpg ").c_str(), GL_REPEAT);
		Textures::map["YELLOW"] = texture;
	}
	{
		Texture2D* texture = new Texture2D(); 
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Red.png").c_str(), GL_REPEAT);
		Textures::map["RED"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Orange.jpg").c_str(), GL_REPEAT);
		Textures::map["ORANGE"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Purple.png").c_str(), GL_REPEAT);
		Textures::map["PURPLE"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Green.jpg").c_str(), GL_REPEAT);
		Textures::map["GREEN"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Mountains.jpg").c_str(), GL_REPEAT);
		Textures::map["NEON_MOUNTAINS"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "Ground.Jpg").c_str(), GL_REPEAT);
		Textures::map["GROUND"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "NeonBlue.Jpg").c_str(), GL_REPEAT);
		Textures::map["NEON_DARK_BLUE"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "NeonYellow.Jpg").c_str(), GL_REPEAT);
		Textures::map["NEON_YELLOW"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "NeonPink.Jpg").c_str(), GL_REPEAT);
		Textures::map["NEON_PINK"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "NeonBlue2.Jpg").c_str(), GL_REPEAT);
		Textures::map["NEON_LIGHT_BLUE"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "CrazyNeon.Jpg").c_str(), GL_REPEAT);
		Textures::map["CRAZY_NEON"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::TEXTURES + "FatigueDecal.png").c_str(), GL_REPEAT);
		Textures::map["FATIGUE"] = texture;
	}
}