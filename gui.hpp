#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "osdialog.h"
#include <string>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "scene.hpp"

class GUI
{
	public:
		GUI(std::string glsl_version, GLFWwindow *window, Scene *scene);
		void NewFrameSetUp();
		void Render(int width, int height);
		void OneFrame(std::deque<std::string> v_names);
		void ShutDown();
	private:
		GLFWwindow *window;
		Camera *camera;
		Scene *scene;
		std::string currentItemMem;
		
		bool first = true;
		
		std::vector<std::string> v_namesOfModels;
		std::map<std::string, char*> m_sourceCode;
		std::map<std::string, Interpreter> m_interpreters;
		std::map<std::string, bool> m_isRunning;
		void AddComponent(std::string filename, glm::vec3 pos, float mass, float color[3]);
		void AddCube(float size, glm::vec3 pos, float mass, float color[3]);
};