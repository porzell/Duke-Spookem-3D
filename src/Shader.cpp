#include "Shader.h"

//#include <string>
#include <cstring>
#include <fstream>

Shader::Shader()
{
	mShaderID = 0;
}

Shader::~Shader()
{
	deleteShader();
}

Shader::Shader(const char *shaderFilename)
{
	char *vShaderPath = new char[strlen(shaderFilename)+4];
	char *fShaderPath = new char[strlen(shaderFilename)+4];

	strcpy(vShaderPath, shaderFilename);
	strcat(vShaderPath, VERTEX_SHADER_EXTENSION);

	strcpy(fShaderPath, shaderFilename);
	strcat(fShaderPath, FRAGMENT_SHADER_EXTENSION);

	//Run the constructor for multiple paths.
	Shader(vShaderPath, fShaderPath);
}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	std::string vertexShaderData, fragmentShaderData;

	std::ifstream inFile(vertexShaderPath);

	if(inFile)
	{
		inFile.seekg(0, std::ios::end);   
		vertexShaderData.reserve(inFile.tellg());
		inFile.seekg(0, std::ios::beg);

		vertexShaderData.assign((std::istreambuf_iterator<char>(inFile)),
					std::istreambuf_iterator<char>());
	}
	else
		std::cerr << "Vertex shader \"" << vertexShaderPath << "\" not found!\n";

	inFile.close();

	inFile.open(fragmentShaderPath);

	if(inFile)
	{
		inFile.seekg(0, std::ios::end);   
		fragmentShaderData.reserve(inFile.tellg());
		inFile.seekg(0, std::ios::beg);

		fragmentShaderData.assign((std::istreambuf_iterator<char>(inFile)),
					std::istreambuf_iterator<char>());

		inFile.close();
	}
	else
		std::cerr << "Fragment shader \"" << vertexShaderPath << "\" not found!\n";

	mShaderID = glCreateProgram();

	GLuint vertexShader, fragmentShader;

	vertexShader = compileInternalShader(GL_VERTEX_SHADER, vertexShaderData.c_str());
	fragmentShader = compileInternalShader(GL_VERTEX_SHADER, fragmentShaderData.c_str());

	//Attach both of our internal shaders to our shader program.
	glAttachShader(mShaderID, vertexShader);
	glAttachShader(mShaderID, fragmentShader);

	//Now link together the program.
	glLinkProgram(mShaderID);

	//Delete the two internal shaders.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint Shader::compileInternalShader(GLenum shaderType, const char *data)
{
	GLuint shaderID = glCreateShader(shaderType);

	//Load shader from source.  We only want to load one, so we give it a pointer to our c-string pointer.
	glShaderSource(shaderID, 1, &data, NULL);

	//Now compile our shader program.
	glCompileShader(shaderID);

	GLint shaderCompiled;

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);

	if(shaderCompiled == GL_FALSE)
	{
		std::cout << "Error compiling shader!" << std::endl;

		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
 
		//The maxLength includes the NULL character
		char *errorLog = new char[maxLength];

		glGetShaderInfoLog(shaderID, maxLength, &maxLength, errorLog);

		std::cerr << errorLog;

		delete [] errorLog;
	}
	else
		std::cout << "Shader successfully compiled!\n";

	return shaderID;
}

void Shader::deleteShader()
{
	//If our shader has been initialized, allow GL to delete it.
	if(mShaderID)
		glDeleteProgram(mShaderID);
}

void Shader::useShader()
{
	glUseProgram(mShaderID);
	
}
