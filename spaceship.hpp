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
#include "myCube.h"
#include "myTeapot.h"
#include "modelStruct.h"
#include "loadOBJ.hpp"
#include <math.h>



class Spaceship {
public:
	Model model;
	GLuint texture;

	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 vel = glm::vec3(0, 0, 0);
	float acc = 0.0f;

	glm::vec3 rot = glm::vec3(0, 0, 0);
	glm::vec3 rot_vel = glm::vec3(0, 0, 0);
	glm::vec3 rot_acc = glm::vec3(0, 0, 0);

	glm::vec3 vec_ahead_ss = glm::vec3(0, 0, 1);
	glm::vec3 vec_up_ss = glm::vec3(0, 1, 0);
	glm::vec3 vec_right_ss = glm::vec3(1, 0, 0);

	void init() {
		model = loadOBJ("models/longBlue/longBlue.obj");
		//spaceCraftModel = loadOBJ("models/futuristic/futuristic.obj");
		texture = readTexture("models/longBlue/longBlue.png");
		//tex0 = readTexture("models/futuristic/AircraftS.png");
	}

	void update(float delta) {
		// https://gamedev.stackexchange.com/a/174236
		float friction = 0.1f;
		float frictionFactor = pow(friction, delta);

		rot_vel += rot_acc * delta;
		rot += rot_vel * (frictionFactor - 1) / (float) log(friction);
		rot_vel *= frictionFactor;

		vec_ahead_ss = heading();
		vec_up_ss = vec_up_calc();
		vec_right_ss = vec_right_calc();

		vel += heading() * acc * delta;
		pos += vel * (frictionFactor - 1) / (float)log(friction);
		vel *= frictionFactor;

		std::cout << glm::to_string(vec_up_ss) << std::endl;

		// std::cout << glm::to_string(pos) << std::endl;
	}

	glm::vec3 heading() {
		glm::mat4 rotationMat(1.0f); // Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		//std::cout << glm::to_string(res) << std::endl;
		return res;
	}

	glm::vec3 ceiling() {
		glm::mat4 rotationMat(1.0f); // Creates a identity matrix
		//rotationMat = glm::rotate(rotationMat, -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		//std::cout << glm::to_string(res) << std::endl;
		return res;
	}

	glm::vec3 vec_up_calc() {
		float y_angle = rot.y / 2;
		float x_angle = (rot.x - PI / 2) / 2;

		glm::quat rotationQuatY = glm::quat(cos(y_angle), 0, sin(y_angle), 0);
		glm::quat rotationQuatX = glm::quat(cos(x_angle), sin(x_angle), 0, 0);
		glm::quat rotated = rotationQuatY * rotationQuatX * glm::quat(0.0f, 0.0f, 0.0f, 1.0f) * glm::inverse(rotationQuatY * rotationQuatX);
		//std::cout << glm::to_string(rotated) << std::endl;
		return glm::vec3(rotated.x, rotated.y, rotated.z);

		//glm::mat4 rotationMat(1.0f); // Creates a identity matrix
		//rotationMat = glm::rotate(rotationMat, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		//rotationMat = glm::rotate(rotationMat, rot.x - PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
		//glm::vec3 res = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		//return res;
	}

	glm::vec3 vec_right_calc() {
		glm::quat rotationQuatY = glm::quat(cos((rot.y + PI/2) / 2), 0, sin((rot.y + PI/2) / 2), 0);
		glm::quat rotationQuatX = glm::quat(cos(rot.x / 2), sin(rot.x / 2), 0, 0);
		glm::quat rotated = rotationQuatX * rotationQuatY * glm::quat(0.0f, 0.0f, 0.0f, 1.0f) * glm::inverse(rotationQuatX * rotationQuatY);
		//std::cout << glm::to_string(rotated) << std::endl;
		return glm::vec3(rotated.x, rotated.y, rotated.z);
	}

	void draw(ShaderProgram* sp) {
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, pos);

		// M = glm::rotate(M, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		// M = glm::rotate(M, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

		M = glm::rotate(M, rot.y, vec_up_ss);
		M = glm::rotate(M, rot.x, vec_right_ss);

		//M = glm::rotate(M, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		//M = glm::rotate(M, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

		//M = glm::scale(M, glm::vec3(0.002f, 0.002f, 0.002f));
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

