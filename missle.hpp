#pragma once

#include "Entity.hpp"

extern Model missle_model;
extern GLuint missle_texture;
extern const float DRAW_DISTANCE;

const float VELOCITY = 500.0f;
const float LENGTH_SS = 5.0f;

class Missle : public Entity {
public:
	Missle() {};

	Missle(Model* model, GLuint* texture, Entity* parent) : Entity(model, texture) {
		scale = 1.7f;
		radius *= scale;
		pos = parent->pos + parent->heading() * LENGTH_SS;
		vel = parent->vel + parent->heading() * VELOCITY;
		rot_vel = glm::normalize(glm::ballRand(4.0f)) * 0.2f;
	}

	void update(float delta) {
		update_static(delta);
	}

	void check_distance(Entity* ss) {
		if (distance(ss) > DRAW_DISTANCE) {
			remove = true;
		}
	}

	static Missle new_missle(Entity* parent) {
		return Missle(&missle_model, &missle_texture, parent);
	}
};

