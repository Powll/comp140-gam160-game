#pragma once

#include <glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.h"

class Model
{
	public: 

	Model()
		:	positionAttributeIndex(0)
		,	colorAttributeIndex(1)
	{
	}

	glm::mat4 GetModel()
	{
		return model;
	}

	void SetMatrix(glm::mat4 mvp)
	{
		shader.UseProgram();
		shader.SetMatrix( mvp );
	}

	void Render() const
	{
		shader.UseProgram();
		
		glBindVertexArray(vao[0]);

		glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3 );
	}

	void ResetMatrix()
	{
		model = glm::mat4(1.0f); 
	}

	void Translate(const glm::vec3 &axis, float speed=1.0f)
	{
		model *= glm::translate(axis * speed);
	}

	void Rotate(float degrees, const glm::vec3 &axis)
	{
		model = glm::rotate(model, degrees, axis);
	}

	// Cleanup all the things we bound and allocated
	void CleanUp()
	{
		glDisableVertexAttribArray(0);

		glDeleteBuffers(2, vbo);

		glDeleteVertexArrays(1, vao);

		// Shutdown SDL 2
		SDL_Quit();	
	}

	// This is where we setup the model like we saw in the first part
	bool SetupBufferObjects(std::string inPositions, std::string inColours, int vboIndex = 0, int vaoIndex=0)
	{
		positions = ReadFile(inPositions.c_str());
		colors = ReadFile(inColours.c_str());

		// Generate and assign two Vertex Buffer Objects to our handle
		glGenBuffers(2, vbo);

		// Generate and assign a Vertex Array Object to our handle
		glGenVertexArrays(1, vao);

		// Bind our Vertex Array Object as the current used object
		glBindVertexArray(vao[0]);

		// Colors
		// =======================
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

		// Copy the vertex data from diamond to our buffer
		glBufferData(GL_ARRAY_BUFFER,  colors.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);

		// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
		glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Positions
		// ===================
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

		// Copy the vertex data from diamond to our buffer
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);

		// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
		glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0 );

		// Enable our attributes within the current VAO
		glEnableVertexAttribArray(positionAttributeIndex);
		glEnableVertexAttribArray(colorAttributeIndex);

		// Set up shader 
		// ===================
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(vao[0]);

		return true;
	}

	// A generic function that takes a path as a paramater
	// Returns the information as a char*
	static std::vector<GLfloat> ReadFile(const char* file)
	{
		// Open file
		std::ifstream t(file);

		std::vector<GLfloat> result;
		std::cout << "Reading : " << file << std::endl;

		while (t.good())
		{
			std::string str;
			t >> str;

			GLfloat f = std::atof(str.c_str());

			result.push_back(f);
		}

		return result;
	}

	void SetShader(Shader _shader)
	{
		shader = _shader;
	}

	glm::vec3 GetCenter()
	{
		glm::vec3 center;

		for (int i = 0; i < model.length(); i++)
		{
			if (i == 0)
			{
				center.x += model[i].x;
				center.y += model[i].y;
				center.z += model[i].z;
			}
			else
			{
				center.x = (center.x + model[i].x) / 2;
				center.y = (center.y + model[i].y) / 2;
				center.z = (center.z + model[i].z) / 2;
			}
		}

		return center;
	}

	private:

	// VBO data
	std::vector<GLfloat> positions;
	std::vector<GLfloat> colors;

	// Create variables for storing the ID of our VAO and VBO
	GLuint vbo[2], vao[1]; 

	// The positons of the position and color data within the VAO
	const uint32_t positionAttributeIndex, colorAttributeIndex;

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model;

	Shader shader;
};
