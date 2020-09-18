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
#include "loadOBJ.hpp"
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"

extern ShaderProgram* sp;

class Entity {
public:
	Model* model = nullptr;
	GLuint* texture = nullptr;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f);
	float acc = 0.0f;

	glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 rot_vel = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rot_acc = glm::vec3(0.0f, 0.0f, 0.0f);

	float scale = 1.0f;

	float friction = 1.0f;
	float max_speed = 1;

	bool remove = false;

	Entity() {};

	Entity(Model* model, GLuint* texture) {
		this->model = model;
		this->texture = texture;
	}

	glm::vec3 heading() {
		glm::mat4 rotationMat = glm::mat4(rot);
		glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		return res;
	}

	glm::vec3 ceiling() {
		glm::mat4 rotationMat = glm::mat4(1.0f);
		rotationMat *= glm::toMat4(rot);
		rotationMat = glm::rotate(rotationMat, -PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		return res;
	}

	float distance(Entity* e) {
		return glm::distance(e->pos, pos);
	}

	virtual void update(float) = 0;

	void update_static(float delta) {
		pos += vel * delta;

		auto curr_rot = rot_vel * delta;

		rot = glm::rotate(rot, curr_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rot = glm::rotate(rot, curr_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rot = glm::rotate(rot, curr_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

		rot = glm::normalize(rot);
	}

	void update_dynamic(float delta) {
		// https://gamedev.stackexchange.com/a/174236
		float frictionFactor = pow(friction, delta);

		rot_vel += rot_acc * delta;
		auto curr_rot = rot_vel * (frictionFactor - 1) / (float)log(friction);
		rot_vel *= frictionFactor;

		// rot = glm::rotate(rot, 1.0f, curr_rot);

		rot = glm::rotate(rot, curr_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rot = glm::rotate(rot, curr_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rot = glm::rotate(rot, curr_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

		rot = glm::normalize(rot);

		vel += heading() * acc * delta;
		pos += vel * (frictionFactor - 1) / (float)log(friction);
		vel *= frictionFactor;

		max_speed = (glm::length(vel) > max_speed) ? glm::length(vel) : max_speed;
	}

	void draw() {
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, pos);
		M *= glm::toMat4(rot);
		M = glm::scale(M, glm::vec3(scale, scale, scale));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

		glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &(model->verticies[0])); //Specify source of the data for the attribute vertex

		glEnableVertexAttribArray(sp->a("normal")); //Enable sending data to the attribute vertex
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &(model->vertexNormals[0])); //Specify source of the data for the attribute vertex

		glEnableVertexAttribArray(sp->a("texCoord0")); //Enable sending data to the attribute color
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &(model->texCoords[0])); //Specify source of the data for the attribute color

		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *texture);

		glDrawArrays(GL_TRIANGLES, 0, model->vertexCount); //Draw the object

		glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
		glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal
		glDisableVertexAttribArray(sp->a("texCoord0")); //Disable sending data to the attribute texCoord0
	}
};
