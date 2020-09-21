#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Model {
	int vertexCount;
	std::vector<float> verticies;
	std::vector<float> colors;
	std::vector<float> sideNormals;
	std::vector<float> vertexNormals;
	std::vector<float> texCoords;
};