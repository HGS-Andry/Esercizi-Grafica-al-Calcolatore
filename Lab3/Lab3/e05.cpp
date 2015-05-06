//Andrea Dall'Alba VR378774


// win : CL /MD /Feapp /Iinclude e04.cpp /link /LIBPATH:lib\win /NODEFAULTLIB:MSVCRTD
// osx : c++ -std=c++11 -o app e04.cpp -I ./include -L ./lib/mac -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
// lin : c++ -std=c++0x -o app e04.cpp -I ./include -L./lib/lin -Wl,-rpath,./lib/lin/ -lglfw -lGL
// l32 : c++ -std=c++0x -o app e04.cpp -I ./include -L./lib/lin32 -Wl,-rpath,./lib/lin32/ -lglfw -lGL

#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#define PI 3.1415926535897932384626433832795f

using timer = std::chrono::high_resolution_clock;

#ifdef _WIN32
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>
#include <lodepng.hpp>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if defined(_MSC_VER)
#pragma comment(lib,"user32")
#pragma comment(lib,"gdi32")
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glew32")
#pragma comment(lib,"glfw3")
#endif

//Definisco parametri della camera modificabili interattivamente
glm::vec3 camera_position(0.0f, 0.0f, -5.0f);
glm::vec3 camera_direction(0.0f, 0.0f, 1.0f);
glm::vec3 camera_up(0.0f, 1.0f, 0.0f);

bool	translate_forward=false;
bool	translate_backward=false;
bool	translate_right=false;
bool	translate_left=false;
bool	rotate_up=false;
bool	rotate_down=false;
bool	rotate_right=false;
bool	rotate_left=false;
bool	rotate_clock = false;
bool	rotate_counterclock = false;

int		switched = 0;
bool	moved = false;
glm::mat4 cube3position = glm::translate(glm::mat4(1.f), glm::vec3(3.0f, 3.0f, 3.0f));

static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);



	if(key == GLFW_KEY_W)
	{
		if(action)
			translate_forward = true;
		else
			translate_forward = false;
	}
	if(key == GLFW_KEY_S)
	{
		if(action)
			translate_backward = true;
		else
			translate_backward = false;
	}
	if(key == GLFW_KEY_D)
	{
		if(action)
			translate_right = true;
		else
			translate_right = false;
	}
	if(key == GLFW_KEY_A)
	{
		if(action)
			translate_left = true;
		else
			translate_left = false;
	}
	if(key == GLFW_KEY_UP)
	{
		if(action)
			rotate_up = true;
		else
			rotate_up = false;
	}
	if(key == GLFW_KEY_DOWN)
	{
		if(action)
			rotate_down = true;
		else
			rotate_down = false;
	}
	if(key == GLFW_KEY_RIGHT)
	{
		if(action)
			rotate_right = true;
		else
			rotate_right = false;
	}
	if(key == GLFW_KEY_LEFT)
	{
		if(action)
			rotate_left = true;
		else
			rotate_left = false;
	}
	if (key==GLFW_KEY_Q)
	{
		if (action)
			rotate_counterclock = true;
		else
			rotate_counterclock = false;
	}
	if (key == GLFW_KEY_E)
	{
		if (action)
			rotate_clock = true;
		else
			rotate_clock = false;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		if (switched == 0)
			switched = 1;
		else
			switched = 0;
	}
	if (key == GLFW_KEY_O)
	{
		moved = true;
	}

// Qui si possono aggiungere interazioni con tasti
}

// Shader sources
const GLchar* vertexSource =
#if defined(__APPLE_CC__)
	"#version 150 core\n"
#else
	"#version 130\n"
#endif
	"in vec3 position;"
	"in vec3 color;"
	"in vec2 coord;"
	"out vec3 Color;"
	"out vec2 Coord;"
	"uniform mat4 model;"
	"uniform mat4 view;"
	"uniform mat4 projection;"
	"void main() {"
	"	Color = color;"
	"	Coord = coord;"
	"	gl_Position = projection * view * model * vec4(position, 1.0);"
	"}";

const GLchar* fragmentSource =
#if defined(__APPLE_CC__)
	"#version 150 core\n"
#else
	"#version 130\n"
#endif
	"in vec3 Color;"
	"in vec2 Coord;"
	"out vec4 outColor;"
	"uniform sampler2D textureSampler;"
	"uniform sampler2D textureSampler2;"
	"void main() {"
	"	outColor = vec4(Color, 1.0)*mix(texture(textureSampler, Coord),texture(textureSampler2, Coord),0.2f);"
// Qui si modifica se si vuole fare mix (o provare altro!)
	"}";

const GLfloat vertices[] = {
//	Position				Color				Texcoords
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	.25f, 0.0f, // 0
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.5f, 0.0f, // 1

	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 1.f/3.f, // 2
	-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,	.25f, 1.f/3.f, // 3
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,	0.5f, 1.f/3.f, // 4
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	.75f, 1.f/3.f, // 5
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	1.0f, 1.f/3.f, // 6

	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 2.f/3.f, // 7
	-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f,	.25f, 2.f/3.f, // 8
	 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f,	0.5f, 2.f/3.f, // 9
	 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	.75f, 2.f/3.f, // 10
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	1.0f, 2.f/3.f, // 11

	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	.25f, 1.0f, // 12
	 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.5f, 1.0f, // 13
};
/* 0--1
   |  |
2--3--4--5--6
|  |  |  |  |
7--8--9-10-11
   |  |
  12--13    */
const GLuint elements[] = {
	0, 3, 4,  0, 4, 1,
	2, 7, 8,  2, 8, 3,
	3, 8, 9,  3, 9, 4,
	4, 9,10,  4,10, 5,
	5,10,11,  5,11, 6,
	8,12,13,  8,13, 9
};

GLuint vao;
GLuint vbo;
GLuint ibo;
GLuint shaderProgram, shaderProgram2;
GLuint textures[3];
timer::time_point start_time, last_time;
int frameCount;
float fps;

float t = 0.0;
float dt = 0.0;

void check(int line)
{
	GLenum error = glGetError();
	while(error != GL_NO_ERROR)
	{
		switch(error)
		{
			case GL_INVALID_ENUM: std::cout << "GL_INVALID_ENUM : " << line << std::endl; break;
			case GL_INVALID_VALUE: std::cout << "GL_INVALID_VALUE : " << line << std::endl; break;
			case GL_INVALID_OPERATION: std::cout << "GL_INVALID_OPERATION : " << line << std::endl; break;
			case GL_OUT_OF_MEMORY: std::cout << "GL_OUT_OF_MEMORY : " << line << std::endl; break;
			default: std::cout << "Unrecognized error : " << line << std::endl; break;
		}
		error = glGetError();
	}
}

void initialize_shader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void destroy_shader()
{
	glDeleteProgram(shaderProgram);
}

void initialize_vao()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// shaderProgram must be already initialized
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint cooAttrib = glGetAttribLocation(shaderProgram, "coord");
	glEnableVertexAttribArray(cooAttrib);
	glVertexAttribPointer(cooAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
}

void destroy_vao()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);
}

void initialize_texture()
{
	glGenTextures(3, textures);
	std::vector<unsigned char> image;
	std::vector<unsigned char> image2;
	std::vector<unsigned char> image3;
	unsigned width, height;

	
	
	//Prima texture
	unsigned error = lodepng::decode(image, width, height, "cube.png");
	if(error) std::cout << "decode error " << error << ": " << lodepng_error_text(error) << std::endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//Seconda Texture
	error = lodepng::decode(image2, width, height, "cube2.png");
	if (error) std::cout << "decode error " << error << ": " << lodepng_error_text(error) << std::endl;
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Terza Texture
	error = lodepng::decode(image3, width, height, "cube3.png");
	if (error) std::cout << "decode error " << error << ": " << lodepng_error_text(error) << std::endl;
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void 	destroy_texture()
{
	glDeleteTextures(3, textures);
}


// aggiorniamo la posizione della camera in base ai tasti premuti e pesiamo le trasformazioni per il tempo trascorso dall'ultimo frame (dt) per renderle indipendenti dalla velocita' di disegno della scheda grafica
void update_camera()
{
	glm::vec3 right = glm::cross(camera_direction, camera_up);
	if(translate_forward)
	{
		camera_position += camera_direction*dt;
	}
	if(translate_backward)
	{
		camera_position -= camera_direction*dt;
	}
	if(translate_right)
	{
		camera_position += right * dt;
	}
	if(translate_left)
	{
		camera_position -= right * dt;
	}
	if(rotate_up)
	{
		camera_direction = glm::rotate(camera_direction, dt, right);
		camera_up = glm::rotate(camera_up, dt, right);
	}
	if(rotate_down)
	{
		camera_direction = glm::rotate(camera_direction, -dt, right);
		camera_up = glm::rotate(camera_up, -dt, right);
	}
	if(rotate_right)
	{
		camera_direction = glm::rotate(camera_direction, -dt, camera_up);
	}
	if(rotate_left)
	{
		camera_direction = glm::rotate(camera_direction, dt, camera_up);
	}

	//aggiunta la rotazione
	if (rotate_clock)
	{
		camera_up = glm::rotate(camera_up, dt, camera_direction);
	}
	if (rotate_counterclock)
	{
		camera_up = glm::rotate(camera_up, -dt, camera_direction);
	}
}

void draw(GLFWwindow* window)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
 	frameCount++;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shaderProgram);

	
	//Calcolo il tempo
	t = (timer::now()-start_time).count() * (float(timer::period::num)/float(timer::period::den));
	dt=(timer::now()-last_time).count() * (float(timer::period::num)/float(timer::period::den));
	last_time = timer::now();

	//modifico la visuale
	update_camera();
	glm::mat4 projection = glm::perspective(PI/3.f, float(width)/float(height), 0.01f, 25.0f);
	glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);


	//Cubo uno che ruota
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0 + switched);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2"), 1 - switched);

	glm::mat4 model = glm::rotate(glm::mat4(1.f), t, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_INT, 0); // facce * triangoli per faccia * vertici per triangolo


	//Cubo due
	model = glm::translate(glm::mat4(1.f),glm::vec3(1.0f, 1.0f, 2.0f));


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 1 - switched);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2"), 0 + switched);
	glDrawElements(GL_TRIANGLES, 6*3*2, GL_UNSIGNED_INT, 0); // facce * triangoli per faccia * vertici per triangolo

	//Cubo tre
	//prendo il primo valore del vettore di traslazione della matrice di posizione del cubo3 e controllo sia superiore a 0
	if (moved && cube3position[3][0] >= 0.0f)
	{
		cube3position = glm::translate(cube3position, glm::vec3(dt, -dt, dt));
	}

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &cube3position[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 2);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler2"), 2);
	glDrawElements(GL_TRIANGLES, 6 * 3 * 2, GL_UNSIGNED_INT, 0); // facce * triangoli per faccia * vertici per triangolo

}

int main(int argc, char const *argv[])
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if(!glfwInit())
		return EXIT_FAILURE;

#if defined(__APPLE_CC__)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
	window = glfwCreateWindow(800, 800, "cg-lab", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

#if defined(_MSC_VER)
	glewExperimental = true;
	if (glewInit() != GL_NO_ERROR)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
#endif

	glfwSetKeyCallback(window, key_callback);

	initialize_shader(); check(__LINE__);
	initialize_vao(); check(__LINE__);
	initialize_texture(); check(__LINE__);

	start_time = timer::now();

//Qui si attiva lo z-buffer!
	glEnable(GL_DEPTH_TEST); check(__LINE__);
//qui si attiva il back face culling
	//glEnable(GL_CULL_FACE); check(__LINE__);

	//inizializzo la dimensione e la rotazione del cubo
	cube3position = glm::scale(cube3position, glm::vec3(0.5, 0.5, 0.5));
	cube3position = glm::rotate(cube3position, PI, glm::vec3(0.0f, 1.0f, 0.0f));

	while(!glfwWindowShouldClose(window))
	{
		draw(window); check(__LINE__);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy_vao(); check(__LINE__);
	destroy_shader(); check(__LINE__);
	destroy_texture(); check(__LINE__);

	glfwDestroyWindow(window);

	glfwTerminate();
	return EXIT_SUCCESS;
}
