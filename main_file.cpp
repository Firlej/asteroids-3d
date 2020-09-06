/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define GLM_FORCE_RADIANS

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

#include "spaceship.hpp"

const float ROTATION_VELOCITY = PI;
const float ACCELERATION = 10.0f;

float aspectRatio = 1;
ShaderProgram* sp; //Pointer to the shader program

Spaceship ss; // spaceship

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (action) {
		case GLFW_PRESS: {
			if (key == GLFW_KEY_LEFT) ss.rot_acc.y = ROTATION_VELOCITY;
			if (key == GLFW_KEY_RIGHT) ss.rot_acc.y = -ROTATION_VELOCITY;
			if (key == GLFW_KEY_UP) ss.rot_acc.x = -ROTATION_VELOCITY;
			if (key == GLFW_KEY_DOWN) ss.rot_acc.x = ROTATION_VELOCITY;
			if (key == GLFW_KEY_W) ss.acc.z = ACCELERATION;
			if (key == GLFW_KEY_S) ss.acc.z = -ACCELERATION;
			break;
		}
		case GLFW_RELEASE: {
			if (key == GLFW_KEY_LEFT) ss.rot_acc.y = 0;
			if (key == GLFW_KEY_RIGHT) ss.rot_acc.y = 0;
			if (key == GLFW_KEY_UP) ss.rot_acc.x = 0;
			if (key == GLFW_KEY_DOWN) ss.rot_acc.x = 0;
			if (key == GLFW_KEY_W) ss.acc.z = 0;
			if (key == GLFW_KEY_S) ss.acc.z = 0;
			break;
		}
		default: {
			//
		}
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");

	ss.init();
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	delete sp;
}

// run updates on all objects
void update_all(float delta) {
	ss.update(delta);
}

//Drawing procedure
void drawScene(GLFWwindow* window) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -10.0f), // camera located at
		glm::vec3(0.0f, 0.0f, 0.0f), // looking at
		glm::vec3(0.0f, 1.0f, 0.0f) // up vector
	); 
	glm::mat4 P = glm::perspective(60.0f * PI / 180.0f, 1.0f, 1.0f, 10000.0f); //compute projection matrix

	sp->use(); //activate shading program

	//Send parameters to graphics card
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	ss.draw(sp);

	glfwSwapBuffers(window); //Copy back buffer to front buffer
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	//Create a window titled "OpenGL" and an OpenGL context associated with it.
	window = glfwCreateWindow(1000, 1000, "Asteroids 3D", NULL, NULL);

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure

	glfwSetTime(0); //Zero the timer

	//Main application loop
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		update_all(glfwGetTime());
		glfwSetTime(0);

		drawScene(window); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}

