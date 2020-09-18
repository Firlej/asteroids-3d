#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "modelStruct.h"
#include "loadOBJ.hpp"
#include <math.h>

extern Model asteroid_model;
extern GLuint asteroid_texture;
extern const float DRAW_DISTANCE;

class Asteroid : public Entity {
public:
	Asteroid() {};
	Asteroid(Model* model, GLuint* texture) : Entity(model, texture) {
		pos = glm::ballRand(DRAW_DISTANCE / 10);
		vel = glm::ballRand(4.0f);
	};

	static Asteroid new_asteroid() {
		return Asteroid(&asteroid_model, &asteroid_texture);
	}
};
