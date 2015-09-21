#pragma once
#ifndef _SHADER_H
#define _SHADER_H

#include "GLincludes.h"

#include "Trackable.h"

//Made while examining https://github.com/Dimi814/SimpleFPS/blob/master/SimpleFPS/ShaderLoader.cpp

//Default vertex shader extension.
const char VERTEX_SHADER_EXTENSION[] = ".vsh";

//Default fragment shader extension.
const char FRAGMENT_SHADER_EXTENSION[] = ".fsh";

class Shader : public Trackable
{
	private:

		GLuint mShaderID;

		//Compile an internal shader from shader code.
		GLuint compileInternalShader(GLenum shaderType, const char *data);

    public:
		//Default constructor.
		Shader();

		//Default destructor.
		~Shader();

		//Constructor for loading a shader from a path name. (Assumes vertex shader ends in .vsh and fragment shader ends in .fsh)
		Shader(const char *shaderFilename);

		//Constructor for loading shader from a vertex shader path and a fragment shader path.
		Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

		//Delete shader if exists.
		void deleteShader();

		//Use shader.
		void useShader();

};

#endif