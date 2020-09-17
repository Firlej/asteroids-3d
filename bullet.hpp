#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>

const float VELOCITY = 5.0f;
const float LENGTH_SS = 5.0f;

class Bullet {
public:
	Model model;
	GLuint texture;

	glm::vec3 pos;
	glm::vec3 vel;

	bool bool_draw;

	void init() {
		this->pos = glm::vec3(0.0f);
		this->vel = glm::vec3(0.0f);
		this->bool_draw = false;

		model = loadOBJ("models/bulletBall/bulletBall.obj");
		texture = readTexture("models/bulletBall/bulletBall.png");
	}

	void fire(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 direction, glm::vec3 rot_vel0) {
		this->pos = pos0 + direction * LENGTH_SS;
		this->vel = vel0 + direction * VELOCITY;

		bool_draw = true;
	}
	
	void update(float delta) {
		pos += vel * delta;
	}

	void draw(ShaderProgram* sp) {
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, pos);
		M = glm::scale(M, glm::vec3(0.3f, 0.3f, 0.3f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

		glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &(model.verticies[0])); //Specify source of the data for the attribute vertex

		glEnableVertexAttribArray(sp->a("normal")); //Enable sending data to the attribute vertex
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &(model.vertexNormals[0])); //Specify source of the data for the attribute vertex

		glEnableVertexAttribArray(sp->a("texCoord0")); //Enable sending data to the attribute color
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &(model.texCoords[0])); //Specify source of the data for the attribute color

		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount); //Draw the object

		glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
		glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal
		glDisableVertexAttribArray(sp->a("texCoord0")); //Disable sending data to the attribute texCoord0
	}
};
