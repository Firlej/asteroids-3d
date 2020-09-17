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


class Entity {
public:
	Model* model = nullptr;
	GLuint* texture = nullptr;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.000001f);
	float acc = 0.0f;

	glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 rot_vel = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rot_acc = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	float distance(glm::vec3 pos) {
		return glm::length(this->pos - pos);
	}

	Entity() {};

	Entity(Model* model, GLuint* texture) {
		this->model = model;
		this->texture = texture;
	}

	virtual void update(float) = 0;

	void draw(ShaderProgram* sp) {
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, pos);
		M *= glm::toMat4(rot);
		M = glm::scale(M, scale);

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
