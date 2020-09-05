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
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "modelStruct.h"
#include "loadOBJ.hpp"

const float ROTATION_VELOCITY = PI;
const float VELOCITY = 10.0f;

float rot_vel_x = 0;
float rot_vel_y = 0;
float vel = 0; //angular speed in radians
float aspectRatio = 1;
ShaderProgram* sp; //Pointer to the shader program
Model spaceCraftModel;
GLuint tex0;
GLuint tex1;

//Uncomment to draw a cube
float* vertices;
float* texCoords;
float* normals;
int vertexCount;

//Uncomment to draw a cube
//float* vertices=myCubeVertices;
//float* texCoords= myCubeTexCoords;
//float* colors = myCubeColors;
//float* normals = myCubeNormals;
//int vertexCount = myCubeVertexCount;

//Uncomment to draw a teapot
//float* vertices = myTeapotVertices;
//float* texCoords = myTeapotTexCoords;
//float* colors = myTeapotColors;
//float* normals = myTeapotVertexNormals;
//int vertexCount = myTeapotVertexCount;


//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) rot_vel_x = ROTATION_VELOCITY;
		if (key == GLFW_KEY_RIGHT) rot_vel_x = -ROTATION_VELOCITY;
		if (key == GLFW_KEY_UP) rot_vel_y = -ROTATION_VELOCITY;
		if (key == GLFW_KEY_DOWN) rot_vel_y = ROTATION_VELOCITY;
		if (key == GLFW_KEY_W) vel = VELOCITY;
		if (key == GLFW_KEY_S) vel = -VELOCITY;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) rot_vel_x = 0;
		if (key == GLFW_KEY_RIGHT) rot_vel_x = 0;
		if (key == GLFW_KEY_UP) rot_vel_y = 0;
		if (key == GLFW_KEY_DOWN) rot_vel_y = 0;
		if (key == GLFW_KEY_W) vel = 0;
		if (key == GLFW_KEY_S) vel = 0;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

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
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	spaceCraftModel = loadOBJ("models/longBlue/longBlue.obj");
	//spaceCraftModel = loadOBJ("models/futuristic/futuristic.obj");
	tex0 = readTexture("models/longBlue/longBlue.png");
	//tex0 = readTexture("models/futuristic/AircraftS.png");

	vertices = &spaceCraftModel.verticies[0];
	texCoords = &spaceCraftModel.texCoords[0];
	normals = &spaceCraftModel.vertexNormals[0];
	vertexCount = spaceCraftModel.vertexCount;
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Place any code here that needs to be executed once, after the main loop ends************
	delete sp;
}

//Drawing procedure
void drawScene(GLFWwindow* window, float distance_z, float angle_x, float angle_y) {
	//************Place any code here that draws something inside the window******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -20.0f), // camera located at (x, y, z)
		glm::vec3(0.0f, 0.0f, 0.0f), // looking at (x, y, z)
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vector
	glm::mat4 P = glm::perspective(60.0f * PI / 180.0f, 1.0f, 1.0f, 100.0f); //compute projection matrix

	sp->use();//activate shading program
	//Send parameters to graphics card
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));


	//M = glm::translate(M, glm::vec3(0.0f, -10.0f, 0.0f));
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(0.0f, -10.0f, distance_z + 20.0f));

	M = glm::rotate(M, angle_x + PI, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, angle_y - PI / 3, glm::vec3(1.0f, 0.0f, 0.0f));

	M = glm::scale(M, glm::vec3(0.003f, 0.003f, 0.003f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("normal")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("texCoord0")); //Enable sending data to the attribute color
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Specify source of the data for the attribute color

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Draw the object

	glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal
	glDisableVertexAttribArray(sp->a("texCoord0")); //Disable sending data to the attribute texCoord0

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

	window = glfwCreateWindow(1000, 1000, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it.

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


	float distance_z = 0; //current rotation position of the object, z axis
	float angle_x = 0; //current rotation angle of the object, x axis
	float angle_y = 0; //current rotation angle of the object, y axis
	glfwSetTime(0); //Zero the timer
	//Main application loop
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		distance_z += vel * glfwGetTime(); //Add angle by which the object was rotated in the previous iteration
		angle_x += rot_vel_x * glfwGetTime(); //Add angle by which the object was rotated in the previous iteration
		angle_y += rot_vel_y * glfwGetTime(); //Add angle by which the object was rotated in the previous iteration
		glfwSetTime(0); //Zero the timer
		drawScene(window, distance_z, angle_x, angle_y); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}

