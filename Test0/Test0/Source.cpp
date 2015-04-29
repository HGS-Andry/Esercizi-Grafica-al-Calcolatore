#include <cstdlib>
#include <iostream>


#ifdef _WIN32
//#include <GL/glew.h>
#include "../../librerie\include\GL/glew.h"
#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif
#include "../../librerie\include\GLFW/glfw3.h"
//#include <GLFW/glfw3.h>

#if defined(_MSC_VER)
#pragma comment(lib,"user32")
#pragma comment(lib,"gdi32")
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glew32")
#pragma comment(lib,"glfw3")
#endif


int main(int argc, char **argv)
{
	return 0;
}