#pragma once
#include <Core/Engine.h>

void createCircle(glm::vec3 center, std::vector<VertexFormat>* vertices, std::vector<unsigned short>* indices) {
	glm::vec3 color(0);
	glm::vec3 normal(0, 0, -1);
	vertices->push_back(VertexFormat(center, color, normal, glm::vec2(0.5, 0.5)));
	unsigned short centerPos = vertices->size() - 1;

	float radius = 1;
	float i;
	float interval = 0.1f;
	// upper semicircle
	for (i = 0; i <= 2 * radius; i += interval) {
		float x = -radius + i;
		float y = sqrt(radius * radius - x * x);
		glm::vec2 texCoord((1 + x) / 2, (1 + y) / 2);
		vertices->push_back(VertexFormat(center + glm::vec3(x, y, 0), color, normal, texCoord));
	}

	// lower semicircle
	for (i = 2 * radius; i > 0; i -= interval) {
		float x = -radius + i;
		float y = sqrt(radius * radius - x * x);
		glm::vec2 texCoord((1 + x) / 2, (1 + y) / 2);
		vertices->push_back(VertexFormat(center + glm::vec3(x, -y, 0), color, normal, texCoord));
	}

	int j = centerPos + 1;
	for (; j < vertices->size() - 1; ) {
		indices->push_back(centerPos);
		indices->push_back(j);
		indices->push_back(++j);
	}

	indices->push_back(centerPos);
	indices->push_back(j);
	indices->push_back(centerPos + 1);
}

void createSquare(std::vector<VertexFormat>* vertices, std::vector<unsigned short>* indices) {
	vertices->push_back(VertexFormat(glm::vec3(-0.5, -0.5, 0), glm::vec3(0), glm::vec3(0, 0, 1), glm::vec2(0, 0)));
	vertices->push_back(VertexFormat(glm::vec3(0.5, -0.5, 0), glm::vec3(0), glm::vec3(0, 0, 1), glm::vec2(1, 0)));
	vertices->push_back(VertexFormat(glm::vec3(0.5, 0.5, 0), glm::vec3(0), glm::vec3(0, 0, 1), glm::vec2(1, 1)));
	vertices->push_back(VertexFormat(glm::vec3(-0.5, 0.5, 0), glm::vec3(0), glm::vec3(0, 0, 1), glm::vec2(0, 1)));

	int start = indices->size();
	indices->push_back(start);
	indices->push_back(start + 1);
	indices->push_back(start + 2);
	indices->push_back(start);
	indices->push_back(start + 2);
	indices->push_back(start + 3);
}

void createPyramid(std::vector<VertexFormat>* vertices, std::vector<unsigned short>* indices) {
	createSquare(vertices, indices);

	vertices->push_back(VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 0, -1), glm::vec2(0.5, 0.5)));

	for (int i = 0; i < 3; i++) {
		indices->push_back(i);
		indices->push_back(4);
		indices->push_back(i + 1);
	}
}

void createCilinder(std::vector<VertexFormat>* vertices, std::vector<unsigned short>* indices) {
	createCircle(glm::vec3(0), vertices, indices);
	createCircle(glm::vec3(0,0,1), vertices, indices);

	int i = vertices->size() + 1;
	int halfPoint = vertices->size() / 2;
	vertices->insert(vertices->end(), vertices->begin(), vertices->end());

	for (int j = vertices->size() / 2; j < vertices->size() / 4 * 3; ) {
		(*vertices)[j++].text_coord = glm::vec2(0);
		(*vertices)[j++].text_coord = glm::vec2(1, 0);
	}

	vertices->insert(vertices->end(), (*vertices)[1]);
	vertices->insert(vertices->end(), (*vertices)[halfPoint + 1]);
	for (int j = vertices->size() / 4 * 3; j < vertices->size() - 2; ) {
		(*vertices)[j++].text_coord = glm::vec2(0, 1);
		(*vertices)[j++].text_coord = glm::vec2(1);
	}
	(*vertices)[vertices->size() - 2].text_coord = glm::vec2(1, 0);
	(*vertices)[vertices->size() - 1].text_coord = glm::vec2(1);

	for (; i < vertices->size() / 4 * 3 - 1; i++) {
		indices->push_back(i);
		indices->push_back(i + halfPoint);
		indices->push_back(i + halfPoint + 1);

		indices->push_back(i);
		indices->push_back(i + halfPoint + 1);
		indices->push_back(i + 1);
	}
	indices->push_back(vertices->size() - 1);
	indices->push_back(i + halfPoint);
	indices->push_back(i);


	indices->push_back(vertices->size() - 1);
	indices->push_back(vertices->size() - 2);
	indices->push_back(i);
}