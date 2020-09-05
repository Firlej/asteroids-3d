#pragma once

#include "loadOBJ.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Load into computer's memory
	std::vector<unsigned char> image;   //Allocate a vector for image storage
	unsigned width, height;   //Variables for image size
	//Read image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import into graphics card's memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate the handle
	//Import image into graphics card's memory associated with the handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

class Spaceship {
public:
	Model model;
	GLuint texture;

	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 vel = glm::vec3(0, 0, 0);
	glm::vec3 acc = glm::vec3(0, 0, 0);

	glm::vec2 rot = glm::vec2(0, 0);
	glm::vec2 rot_vel = glm::vec2(0, 0);
	glm::vec2 rot_acc = glm::vec2(0, 0);

	void init() {
		model = loadOBJ("models/longBlue/longBlue.obj");
		//spaceCraftModel = loadOBJ("models/futuristic/futuristic.obj");
		texture = readTexture("models/longBlue/longBlue.png");
		//tex0 = readTexture("models/futuristic/AircraftS.png");
	}

	void update(float delta) {
		vel += acc;
		pos += vel * delta;
		acc = glm::vec3(0, 0, 0);

		rot_vel += rot_acc;
		rot += rot_vel * delta;
		rot_acc = glm::vec2(0, 0);
	}
};