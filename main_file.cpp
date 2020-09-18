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
#include <glm/gtc/random.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "modelStruct.h"
#include "loadOBJ.hpp"

#include "tools.hpp"
#include "spaceship.hpp"
#include "asteroid.hpp"

const float ROTATION_VELOCITY = PI;
const float ACCELERATION = 250.0f;
const float DRAW_DISTANCE = 1000.0f;
const int NUM_OF_ASTEROIDS = 50;

float aspectRatio = 1;
ShaderProgram* sp; //Pointer to the shader program

Model spaceship_model;
GLuint spaceship_texture;
Spaceship ss;

Model asteroid_model;
GLuint asteroid_texture;
std::vector<Asteroid> asteroids;

Model missle_model;
GLuint missle_texture;
std::vector<Missle> missles;

auto eye = glm::vec3(0.0f, 0.0f, 0.0f);
auto center = glm::vec3(0.0f, 0.0f, 0.0f);
auto up = glm::vec3(0.0f, 0.0f, 0.0f);

float fov_draw = 0.5f * PI;

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void shoot() {
	missles.push_back(ss.shoot());
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) shoot();
	}
	else if (action == GLFW_RELEASE) {
		//
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) ss.rot_acc.y = ROTATION_VELOCITY;
		if (key == GLFW_KEY_D) ss.rot_acc.y = -ROTATION_VELOCITY;
		if (key == GLFW_KEY_W) ss.rot_acc.x = -ROTATION_VELOCITY;
		if (key == GLFW_KEY_S) ss.rot_acc.x = ROTATION_VELOCITY;
		if (key == GLFW_KEY_UP || key == GLFW_KEY_SPACE) ss.acc = ACCELERATION;
		if (key == GLFW_KEY_DOWN) ss.acc = -ACCELERATION;
		if (key == GLFW_KEY_LEFT_SHIFT) shoot();
	} else if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) ss.rot_acc.y = 0;
		if (key == GLFW_KEY_D) ss.rot_acc.y = 0;
		if (key == GLFW_KEY_W) ss.rot_acc.x = 0;
		if (key == GLFW_KEY_S) ss.rot_acc.x = 0;
		if (key == GLFW_KEY_UP || key == GLFW_KEY_SPACE) ss.acc = 0;
		if (key == GLFW_KEY_DOWN) ss.acc = 0;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void init() {
	spaceship_model = loadOBJ("models/longBlue/longBlue.obj");
	spaceship_texture = readTexture("models/longBlue/longBlue.png");

	asteroid_model = loadOBJ("models/asteroid/asteroid.obj");
	asteroid_texture = readTexture("models/asteroid/asteroid.png");

	missle_model = loadOBJ("models/bulletBall/bulletBall.obj");
	missle_texture = readTexture("models/bulletBall/bulletBall.png");

	ss = Spaceship::new_spaceship();
	for (int i = 0; i < NUM_OF_ASTEROIDS; i++) asteroids.push_back(Asteroid::new_asteroid());
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");

	init();
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	delete sp;
}

// run updates on all objects
void update_all(float delta) {
	ss.update(delta);
	for (Asteroid& a : asteroids) a.update_static(delta);

	for (auto m = missles.begin(); m != missles.end(); m++) {
		if (m->check_distance(&ss))
			missles.erase(m--);
		else
			m->update_static(delta);
	}
}

// run updates on all objects
void draw_all() {
	ss.draw();
	for (Asteroid& a : asteroids) a.draw();
	for (Missle& m : missles) m.draw();
}

//Drawing procedure
void drawScene(GLFWwindow* window, float delta) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float rate = 0.15f;
	eye = lerp(eye, ss.pos - (ss.heading() * 50.0f) + ss.ceiling() * 15.0f, delta, rate);
	center = lerp(center, ss.pos, delta, rate);
	up = lerp(up, ss.ceiling(), delta, rate);

	glm::mat4 V = glm::lookAt(
		eye, // camera located at
		center, // looking at
		up // up vector
	);

	float fov = map(glm::length(ss.vel), 0, ss.max_speed, 0.5f, 0.65f) * PI;
	fov_draw = lerp(fov_draw, fov, delta, 0.1);

	//std::cout << max_speed(ACCELERATION, FRICTION) << " " << glm::length(ss.vel) << " " << fov << std::endl;

	glm::mat4 P = glm::perspective(fov_draw, 1.0f, 1.0f, DRAW_DISTANCE); //compute projection matrix

	sp->use(); //activate shading program

	//Send parameters to graphics card
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	draw_all();

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
		float delta = glfwGetTime();
		glfwSetTime(0);
		update_all(delta);
		drawScene(window, delta); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}

