#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>
#include "Entity.hpp"
#include "spaceship.hpp"

extern Model sky_model;
extern GLuint sky_texture;
extern const float DRAW_DISTANCE;

class Sky : public Entity {
public:
	Entity* parent = nullptr;

	Sky() {};

	Sky(Model* model, GLuint* texture, Entity* _parent) : Entity(model, texture) {
		parent = _parent;
		// todo is there a way to make the sky model share the same pos variable?
		pos = parent->pos;
		scale = 110.0f;
	}

	void update(float delta) {
		pos = parent->pos;
	}

	static Sky new_sky(Entity* parent) {
		return Sky(&sky_model, &sky_texture, parent);
	}
};

