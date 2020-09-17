#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "modelStruct.h"
#include "loadOBJ.hpp"
#include "bullet.hpp"
#include <math.h>
#include "Entity.hpp"

const float FRICTION = 0.1f;

class Spaceship : public Entity {
public:
	float max_speed = 0;

	Spaceship() {};
	Spaceship(Model* model, GLuint* texture) : Entity(model, texture) {
		vel = glm::vec3(0.0f, 0.0f, 0.000001f);
	};

	void update(float delta) {
		// https://gamedev.stackexchange.com/a/174236
		float friction = 0.1f;
		float frictionFactor = pow(friction, delta);

		rot_vel += rot_acc * delta;
		auto curr_rot = rot_vel * (frictionFactor - 1) / (float)log(friction);
		rot_vel *= frictionFactor;

		// rot = glm::rotate(rot, 1.0f, curr_rot);

		rot = glm::rotate(rot, curr_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rot = glm::rotate(rot, curr_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rot = glm::rotate(rot, curr_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

		rot = glm::normalize(rot);

		vel += heading() * acc * delta;
		pos += vel * (frictionFactor - 1) / (float)log(friction);
		vel *= frictionFactor;

		std::cout << glm::length(vel) << std::endl;

		max_speed = (glm::length(vel) > max_speed) ? glm::length(vel) : max_speed;
	}

	glm::vec3 heading() {
		glm::mat4 rotationMat = glm::mat4(rot);
		glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		return res;
	}

	glm::vec3 ceiling() {
		glm::mat4 rotationMat = glm::mat4(1.0f);
		rotationMat *= glm::toMat4(rot);
		rotationMat = glm::rotate(rotationMat, -PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		return res;
	}

	void shoot(Missle *missle) {
		missle->fire(pos, vel, heading(), rot_vel);
	}

	/*float angleBetween(glm::vec3 a, glm::vec3 b) {
		return glm::acos(glm::dot(glm::normalize(a), glm::normalize(b)));
	}*/
};