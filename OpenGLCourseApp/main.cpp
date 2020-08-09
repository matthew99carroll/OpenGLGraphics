#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

// OpenGL rotations use radians, using this allows for working in degrees and easy conversion
const float toRadians = 3.14159265f / 180.0f;

// Create an object for the main window
Window mainWindow;

// List of meshes and shaders
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Location of Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Location of Fragment Shader
static const char* fShader = "Shaders/shader.frag";

/*
CreateObjects is where the creation of meshes is handled. 
Indices and vertices are passed through to a mesh creation 
method and added to the mesh object list.
*/
void CreateObjects() 
{
	// Indices for 3D triangle mesh, 2 sides, front and base i.e. 12 indices
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Vertices for the 3D triangle mesh, 12 vertices total
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Create object meshes
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

/*
CreateShaders is where the creation of shaders is handled.
*/
void CreateShaders()
{
	// Create object shader
	Shader* shader1 = new Shader();

	// Go to directory and read vertex and fragment shader
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

/*
Main rendering loop
*/
int main()
{
	// Draw a window of dimensions 800x600
	mainWindow = Window(800, 600);

	// Runs OpenGL methods to initialize the window
	mainWindow.Initialise();

	// Creates meshes and shaders
	CreateObjects();
	CreateShaders();

	// Uniform variables to be passed to vertex shader. Initialised to 0 by default to prevent weird stuff happening
	GLuint uniformProjection = 0, uniformModel = 0;

	// Create projection matrix. perspective(vertical FOV, Aspect Ratiom, Near clip plane, Far clip Plane)
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() /mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get and handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Bitwise operators to handle what buffers to clear. In this case both Color and Depth buffers are cleared
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Applies the shader to an OpenGL shader program
		shaderList[0].UseShader();

		// Getters for uniform vars model and projection
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		// Create a 4x4 identity matrix as a starting point for transforms
		glm::mat4 model = glm::mat4(1.0f);

		// Apply translation and scaling
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// Pass model and projection to uniform variable counterparts by pointer
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		
		// Render mesh to screen
		meshList[0]->RenderMesh();

		// Create a 4x4 identity matrix as a starting point for transforms
		model = glm::mat4(1.0f);

		// Apply translation and scaling
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// Pass model to uniform variable counterpart by pointer (NB: don't need to do this for projection again)
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Render Mesh to Screen
		meshList[1]->RenderMesh();

		// Set shader program to 0 at end of frame
		glUseProgram(0);

		// Swap buffer to blank empty shader program
		mainWindow.swapBuffers();
	}

	return 0;
}