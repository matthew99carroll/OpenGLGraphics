#include "Mesh.h"

// Mesh Constructor
Mesh::Mesh()
{
	// Vertex array objects, Vertex buffer objects and Index Buffer Objects initalisation
	VAO = 0;
	VBO = 0;
	IBO = 0;

	// Set the index count i.e. number of indicies to an initial value of 0
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numberOfIndices)
{
	// Set index count to the number of indices in the mesh
	indexCount = numberOfIndices;

	// Generate vertex array based on the pass by reference of VAO
	glGenVertexArrays(1, &VAO);

	//Bind Vertex Array to VAO
	glBindVertexArray(VAO);

	// Generate index buffers based on the pass by reference of IBO
	glGenBuffers(1, &IBO);

	// Bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	
	// Creates and initialises index buffer object's data store
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

	// Generate vertex buffers on the pass by reference of VBO
	glGenBuffers(1, &VBO);
	
	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Creates and initalises vertex buffer object's data store
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	// Defines an array of vertex attribute data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Disable vertex attribute array
	glEnableVertexAttribArray(0);

	// Unbind vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind vertex array
	glBindVertexArray(0);

}

void Mesh::RenderMesh()
{
	// Bind VAO and IBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Draw elements
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// Unbind IBO and VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	// Deletes all of the buffers and resets them all to Null
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	// Set the number of indices to 0
	indexCount = 0;
}

// Mesh Destructor
Mesh::~Mesh()
{
	ClearMesh();
}