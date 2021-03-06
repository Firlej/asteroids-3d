#pragma once

#include "Entity.hpp"

extern Model text_model;
extern GLuint text_texture;
extern const float DRAW_DISTANCE;

class Text : public Entity {
public:
	Text() {};

	Text(Model* model, GLuint* texture) : Entity(model, texture) {
		scale = 50.0f;
		pos = glm::vec3(0.0f, -100.0f, 200.0f);
		vel = glm::vec3(0.0f, 70.0f, 70.0f);

	}

	void update(float delta) {
		update_static(delta);
	}

	bool check_distance(Entity* ss) {
		return distance(ss) < DRAW_DISTANCE;
	}

	static Text new_text() {
		return Text(&text_model, &text_texture);
	}
};
