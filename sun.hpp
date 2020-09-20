#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "modelStruct.h"
#include "loadOBJ.hpp"
#include "Entity.hpp"
#include <math.h>

extern Model sun_model;
extern GLuint sun_texture;
extern ShaderProgram* sp_simplest;

const glm::vec3 DIST_FROM_PARENT = glm::vec3(-100.0f, 100.0f, 500.0f);
const glm::vec3 ROTATION = glm::vec3(0.0f, 0.05f, 0.0f);

class Sun : public Entity {
public:

	Entity* parent = nullptr;

	Sun() : Entity() {};

	Sun(Model* model, GLuint* texture, Entity* parent) : Entity(model, texture) {
		this->parent = parent;
		rot_vel = ROTATION;
		scale = 15.0f;
	}

	void update(float delta) {
		pos = parent->pos + DIST_FROM_PARENT;
		update_static(delta);
	}

	static Sun new_sun(Entity* parent) {
		return Sun(&sun_model, &sun_texture, parent);
	}
};
