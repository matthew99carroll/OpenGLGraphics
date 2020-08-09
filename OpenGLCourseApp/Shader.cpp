#include "Shader.h"

// Shader constructor
Shader::Shader()
{
	// Initialise shader ID, and uniform variables for the model and projection to 0
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	// Take code from vertex and fragment shader strings for compiling
	CompileShader(vertexCode, fragmentCode);
}

/// <summary>
/// Takes in a .vert and .frag shader file location and converts it to code for compiling
/// </summary>
void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

/// <summary>
/// Takes in a file location and reads shader file line by line and places into a string
/// </summary>
std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	// Creates an empty program object
	shaderID = glCreateProgram();

	// Error checking -> Checks if invalid shader ID is passed 
	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	// Add vertex and fragment shaders
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	// initalise result to null or 0 (this will later store the program object for the shader)
	GLint result = 0;

	// Char array to store error log
	GLchar eLog[1024] = { 0 };

	// Links a program object when provided with the handle of the program object
	glLinkProgram(shaderID);

	// Returns a parameter from a program object link and stores it as a reference in result
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	// Error checking -> Checks if the result of the program object link is valid
	if (!result)
	{
		// Fill error char array and print result
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	// Run a validation check to ensure program will compile and run
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

	// Error checking -> Checks if the result of the program object validation is valid
	if (!result)
	{
		// Fill error char array and print result
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	// Returns the uniform variables for object and projection stored in the vertex shader
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
}

/// <summary>
/// Projection uniform variable getter
/// </summary>
GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

/// <summary>
/// Model uniform variable getter
/// </summary>
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

/// <summary>
/// View uniform variable getter
/// </summary>
GLuint Shader::GetViewLocation()
{
	return uniformView;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	// Deletes shader program data and reinitialises the shader id
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	// Resets the uniform variables
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	// Create shader based on enumerator for shader type such as vertex, fragment, geometry etc
	GLuint theShader = glCreateShader(shaderType);

	// Some whack code not sure how this works I was just told it does lol
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);

	// Compile the shader
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Attempt to compile shader and store the a reference to the result
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	// Error checking -> Check if there was an error compiling the shader
	if (!result)
	{
		// Get the info log and store in the error log char array and print
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the &d shader: '%s'\n", shaderType, eLog);
		return;
	}

	// Attach the shader to the shader program
	glAttachShader(theProgram, theShader);
}

// Shader Destructor
Shader::~Shader()
{
	// Clear Shader
	ClearShader();
}