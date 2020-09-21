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

struct Boundries {
	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 cur_min;
	glm::vec3 cur_max;

	void update(glm::vec3 pos, glm::quat rot) {
		cur_min = min + pos;
		cur_max = max + pos;
	}

	bool collison(Boundries bounds) {
		return (cur_min.x <= bounds.cur_max.x && cur_max.x >= bounds.cur_min.x) &&
			(cur_min.y <= bounds.cur_max.y && cur_max.y >= bounds.cur_min.y) &&
			(cur_min.z <= bounds.cur_max.z && cur_max.z >= bounds.cur_min.z);
	}

	void scale(float scale) {
		min *= scale;
		max *= scale;
	}
};