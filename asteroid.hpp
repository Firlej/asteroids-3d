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
#include "myCube.h"
#include "myTeapot.h"
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
		//
	};

	static Asteroid new_asteroid() {
		Asteroid as = Asteroid(&asteroid_model, &asteroid_texture);
		as.pos = glm::ballRand(DRAW_DISTANCE / 4);
		as.vel = glm::ballRand(4.0f);
		return as;
	}

};

