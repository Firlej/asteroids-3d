#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>
#include "Entity.hpp"

const float VELOCITY = 50.0f;
const float LENGTH_SS = 5.0f;

class Missle : public Entity {
public:
	bool bool_draw = false;

	Missle() {};
	Missle(Model* model, GLuint* texture) : Entity(model, texture) {
		scale = glm::vec3(0.8f, 0.8f, 0.8f);
	}

	void update(float delta) {
		pos += vel * delta;
	}
	
	void fire(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 direction, glm::vec3 rot_vel0) {
		this->pos = pos0 + direction * LENGTH_SS;
		this->vel = vel0 + direction * VELOCITY;

		bool_draw = true;
	}
};
