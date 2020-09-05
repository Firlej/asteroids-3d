#pragma once

//STD Libs
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelStruct.h"


static Model loadOBJ(const char* file_name) {
	//Vertex portions
	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec3> vertex_normals;

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;
	
	Model toReturnModel;
	toReturnModel.vertexCount = 0;

	//File open error check
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	//Read one line at a time
	while (std::getline(in_file, line))
	{
		//Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}
		else if (prefix == "o")
		{

		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "usemtl")
		{

		}
		else if (prefix == "v") //Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			int vertexCounter = 0;
			while (ss >> temp_glint) {

				// Pushing vertex attributes into correct arrays
				if (counter == 0) {
					toReturnModel.verticies.push_back(vertex_positions.at(temp_glint - 1).x);
					toReturnModel.verticies.push_back(vertex_positions.at(temp_glint - 1).y);
					toReturnModel.verticies.push_back(vertex_positions.at(temp_glint - 1).z);
					toReturnModel.verticies.push_back(1.0f);
					toReturnModel.vertexCount++;
				}
				else if (counter == 1) {
					toReturnModel.texCoords.push_back(vertex_texcoords.at(temp_glint - 1).x);
					toReturnModel.texCoords.push_back(vertex_texcoords.at(temp_glint - 1).y);
				}
				else if (counter == 2) {
					toReturnModel.vertexNormals.push_back(vertex_normals.at(temp_glint - 1).x);
					toReturnModel.vertexNormals.push_back(vertex_normals.at(temp_glint - 1).y);
					toReturnModel.vertexNormals.push_back(vertex_normals.at(temp_glint - 1).z);
					toReturnModel.vertexNormals.push_back(0.0f);
				}

				// Handling separators
				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					++vertexCounter;
					ss.ignore(1, ' ');
				}

				// Reset the counter
				if (counter > 2)
					counter = 0;
			}
		}
	}

	return toReturnModel;
}