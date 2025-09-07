#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include "GameStateManager.h"

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int resW = 800;
int resH = 600;

GameStateManager gsm;

Camera camera;
Renderer renderer;

void init();
void render();
void update();
void close();

GLFWwindow* window;

int main()
{

	init();

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do
	{
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			printf("%f ms/frame, %f FPS\n", 1000.0 / double(nbFrames), double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		render();
		update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while (!glfwWindowShouldClose(window));

	close();
}

void init() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
  
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(resW, resH, "Project Timeless", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);
  
	GLenum error = glewInit();
	
	if (error != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	renderer.init(resW, resH);
	camera.init(window);

	gsm.initManager(resW, resH, &renderer, &camera);

	glLineWidth(3);
}

void render() {
	gsm.render();
}

void update() {
	gsm.update();
}

void close() {
	gsm.closeStates();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
