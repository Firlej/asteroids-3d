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
#include "modelStruct.h"
#include "loadOBJ.hpp"
#include "missle.hpp"
#include <math.h>
#include "Entity.hpp"
#include "tools.hpp"

extern Model spaceship_model;
extern GLuint spaceship_texture;

class Spaceship : public Entity {
public:
	Spaceship() {};

	Spaceship(Model* model, GLuint* texture) : Entity(model, texture) {
		friction = 0.1f;
		radius *= scale;
	};

	void update(float delta) {
		update_dynamic(delta);
	}

	Missle shoot() {
		return Missle::new_missle(this);
	}

	static Spaceship new_spaceship() {
		return Spaceship(&spaceship_model, &spaceship_texture);
	}
};
