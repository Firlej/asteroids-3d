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
#include <math.h>

#include "missle.hpp"

extern Model asteroid_model;
extern GLuint asteroid_texture;
extern const float DRAW_DISTANCE;

class Asteroid : public Entity {
public:

	float time_alive = 0.0f;
	float life_span = 100.0f;

	std::vector<Asteroid> children;

	Asteroid() {};

	Asteroid(Model* model, GLuint* texture) : Entity(model, texture) {
		scale = random() * 5;
		bounds.scale(scale);
		pos = glm::ballRand(DRAW_DISTANCE / 3);
		vel = glm::ballRand(10.0f);
		rot_vel = glm::normalize(glm::ballRand(4.0f)) * 0.2f;
		life_span = random() * 20 + 10;
	};

	Asteroid(Model* model, GLuint* texture, Asteroid* parent) : Entity(model, texture) {
		scale = parent->scale * 0.7f;
		bounds.scale(scale);
		pos = parent->pos;
		vel = glm::ballRand(10.0f);
		rot_vel = glm::normalize(glm::ballRand(4.0f)) * 0.2f;
		life_span = random() * 20 + 10;
	};

	void update(float delta) {
		if (remove) return;
		update_static(delta);
		
		/*
		time_alive += delta;
		if (time_alive >= life_span) {
			split();
		}*/
	}

	void split() {
		if (remove) return;
		remove = true;
		if (scale > 1.0f) {
			children.push_back(Asteroid::new_asteroid(this));
			children.push_back(Asteroid::new_asteroid(this));
		}
	}

	static Asteroid new_asteroid() {
		return Asteroid(&asteroid_model, &asteroid_texture);
	}

	static Asteroid new_asteroid(Asteroid* parent) {
		return Asteroid(&asteroid_model, &asteroid_texture, parent);
	}
};
