#pragma once

#include "Entity.hpp"
#include "spaceship.hpp"

extern Model sky_model;
extern GLuint sky_texture;
extern const float DRAW_DISTANCE;

class Sky : public Entity {
public:
	Entity* parent = nullptr;

	Sky() {};

	Sky(Model* model, GLuint* texture, Entity* parent) : Entity(model, texture) {
		this->parent = parent;
		pos = parent->pos;
		scale = 210.0f;
	}

	void update(float delta) {
		pos = parent->pos;
	}

	static Sky new_sky(Entity* parent) {
		return Sky(&sky_model, &sky_texture, parent);
	}
};

