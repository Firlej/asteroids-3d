#pragma once

#include "missle.hpp"
#include "Entity.hpp"

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
