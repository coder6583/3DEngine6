#include <cstdlib>
#include <cmath>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <BulletDynamics/btBulletDynamicsCommon.h>

#include "shader.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "gui.hpp"

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

const GLuint WIDTH = 640;
const GLuint HEIGHT = 480;

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(0.0f, -5.0f, 0.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;

bool firstMouse = true;
bool cursorHidden = true;

Camera camera(cameraPos);
Scene *scene;

const std::string glsl_version = "#version 330";

int main(int argc, char **argv)
{
	if(glfwInit() == GL_FALSE)
	{
  		std::cerr << "Can't initialize GLFW" << std::endl;
    	return 1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  
   GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3D Axis", NULL, NULL);
   
   if(window == NULL)
   {
    // ウィンドウ作成に失敗した処理
   	std::cerr << "Can't create GLFW window." << std::endl;
      return 1;
   }

  // 作成したウィンドウを処理対象とする (GLFW)
   glfwMakeContextCurrent(window);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetKeyCallback(window, key_callback);
   
   glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK)
   {
		std::cout << "Failed to initialize GLEW";
		return -1;
	}
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);		
	glEnable(GL_DEPTH_TEST);
	
// 	glfwMaximizeWindow(window);
	scene = new Scene(window, &camera);

	
	GUI gui(glsl_version, window, scene);
					
  // ウィンドウが開いている間繰り返す
  int nbFrames = 0;
   while(!glfwWindowShouldClose(window))
   {
		glfwPollEvents();
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	
		nbFrames++;
		double fps = (double)nbFrames / (double)deltaTime;	
		nbFrames = 0;
// 		std::cout << fps << std::endl;
		processInput(window);
    	glClearColor(0.2f,0.4f,0.5f, 0.0f);
		glClear(/* GLbitfield mask = */ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Drawing
    	gui.NewFrameSetUp();
    
   	scene -> Draw();
   	gui.OneFrame(scene -> world -> getNames());
   	
   	gui.Render(scene -> width, scene -> height);
   
   		
    	glfwSwapBuffers(window);
	}
	gui.ShutDown();
	delete scene;
	glfwTerminate();
	
   return 0;
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		std::cout << "Saving..." << std::endl;
		scene -> world -> SaveWorld();
		std::cout << "Saved" << std::endl;
	}
	if(cursorHidden)
	{	
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.processKeyboard(FORWARD, deltaTime);
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.processKeyboard(BACKWARD, deltaTime);
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.processKeyboard(LEFT, deltaTime);
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.processKeyboard(RIGHT, deltaTime);
		if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.processKeyboard(UP, deltaTime);
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.processKeyboard(DOWN, deltaTime);
	}
}	

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	if(cursorHidden)
	{
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = ypos - lastY;
	
		lastX = xpos;
		lastY = ypos;
	
		camera.processMouseMovement(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if(cursorHidden)
		{
			cursorHidden = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if(!cursorHidden)
		{
			cursorHidden = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}





















