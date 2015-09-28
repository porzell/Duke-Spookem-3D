#ifndef _GL_INCLUDES_H
#define _GL_INCLUDES_H

#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#ifdef WINDOWS
#include <GL/GLU.h>
#else
#include <GL/glu.h>
#endif

#endif
