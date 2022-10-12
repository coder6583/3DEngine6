#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "SOIL.h"

#include "world.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "shape2dloader.hpp"

class Scene
{
	public:
		Scene(GLFWwindow *window, Camera *camera);
		~Scene();
		void Draw();
		void AddModel(std::string path, glm::vec3 pos = glm::vec3(0.0f), float mass = 1.0f, float color[3] = 0);
		void AddCube(float size, glm::vec3 pos, float mass, float color[3]);
		int width, height = 0;
		
		void changeCameraSpeed(float speed)
		{
			this -> camera -> changeSpeed(speed);
		}
		void changeCameraSensitivity(float sensitivity)
		{
			this -> camera -> changeSensitivity(sensitivity);
		}
		void loadWorld(World world);
		
		int modelNum = 0;
		GLuint groundTex;
		bool isRunning = false;
		
		std::stringstream log;
		World *world;

	private:
		Shader shader = Shader("vertex2.shader", "fragment4.shader");
		Shader defaultShader = Shader("defaultvertex2.shader", "defaultfragment2.shader");
		Shader groundShader = Shader("groundvertex.shader", "groundfragment.shader");
		
		Camera *camera;
		GLFWwindow *window;
		Shape2DLoader *shape;
	
		commandMap m_loopCommand;
};




