#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
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

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Load into computer's memory
	std::vector<unsigned char> image;   //Allocate a vector for image storage
	unsigned width, height;   //Variables for image size
	//Read image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import into graphics card's memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate the handle
	//Import image into graphics card's memory associated with the handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

class Spaceship {
public:
	Model model;
	GLuint texture;

	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 vel = glm::vec3(0, 0, 0);
	glm::vec3 acc = glm::vec3(0, 0, 0); // only one dimension is used

	glm::vec3 rot = glm::vec3(0, 0, 0);
	glm::vec3 rot_vel = glm::vec3(0, 0, 0);
	glm::vec3 rot_acc = glm::vec3(0, 0, 0);

	void init() {
		model = loadOBJ("models/longBlue/longBlue.obj");

		/*std::vector<glm::fvec3> vertex_positions;
		for (auto vp : model.vertex_positions) {
			vertex_positions.push_back(vp * 0.002);
		}*/

		//spaceCraftModel = loadOBJ("models/futuristic/futuristic.obj");
		texture = readTexture("models/longBlue/longBlue.png");
		//tex0 = readTexture("models/futuristic/AircraftS.png");
	}

	void draw(ShaderProgram* sp) {
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, pos + glm::vec3(0.0f, -10.0f, 15.0f));

		M = glm::rotate(M, rot.y + PI, glm::vec3(0.0f, 1.0f, 0.0f));
		M = glm::rotate(M, rot.x - PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));

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

	void update(float delta) {
		// https://gamedev.stackexchange.com/a/174236
		float friction = 0.5;
		float frictionFactor = pow(friction, delta);

		rot_vel += rot_acc * delta;
		rot += rot_vel * (frictionFactor - 1) / (float) log(friction);
		rot_vel *= frictionFactor;

		glm::mat4 rotationMat(1.0f); // Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, -rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 acc_rot = glm::vec3(rotationMat * glm::vec4(acc * delta, 1.0f));
		vel += acc_rot;
		pos += vel * (frictionFactor - 1) / (float)log(friction);
		vel *= frictionFactor;

		std::cout << glm::to_string(pos) << std::endl;
	}
};

