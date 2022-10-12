#include "scene.hpp"

Scene::Scene(GLFWwindow *window, Camera *camera) : shape(new Shape2DLoader)
{
	this -> window = window;
	this -> camera = camera;

	this -> log << "Loading Ground...\n";
	this -> world = new World();
	this -> log << "Loaded Ground\n";
}

Scene::~Scene()
{
	delete this -> world;
}

void Scene::loadWorld(World world)
{
	this -> world = new World(world);
}

void Scene::AddModel(std::string path, glm::vec3 pos, float mass, float color[3])
{
	this -> log << "Loading Model...";
	while(!this -> world -> AddModel(path, pos, mass, color));
	this -> log << "Loaded Model\n";
}

void Scene::AddCube(float size, glm::vec3 pos, float mass, float color[3])
{
	this -> log << "Loading Cube...\n";
	while(!this -> world -> AddCube(size, pos, mass, color));
	this -> log << "Loaded Cube\n";
};

void Scene::Draw()
{
	glfwGetFramebufferSize(window, &this -> width, &this -> height);
	glViewport(0, 0, this -> width, this -> height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, this -> width, 0, this -> height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(isRunning == true)
		this -> world -> getWorld() -> stepSimulation(1.f / 60.f, 10);
	for(int j = this -> world -> getWorld() ->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = this -> world -> getWorld() ->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if(body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		if(j >= 1)
		{
			btTransform t;
			this -> world -> getModels().at(this -> world -> getNames().at(j - 1)) -> body -> getMotionState() -> getWorldTransform(t);
			if(this -> isRunning)
			{
				this -> log << float(roundf(t.getOrigin().getX() * 100)/100) << " " << float(roundf(t.getOrigin().getY() * 100)/100) << " " << float(roundf(t.getOrigin().getZ() * 100)/100) << "\n";
			}
			glm::mat4 tempMat(1.0f);
			t.getOpenGLMatrix(glm::value_ptr(tempMat));
			this -> world -> getModels().at(this -> world -> getNames().at(j - 1)) -> ChangeModelMat(tempMat);
		}
	}

// 	std::cout << this -> world -> getWorld() ->getNumCollisionObjects() << std::endl;
	for(auto &model : this -> world -> getModels())
	{
		model.second -> shader.Use();
		model.second -> MVPUniform(model.second->shader, *(this -> camera), this -> width, this -> height);
		model.second -> Uniform3f(model.second -> shader, "lightPos", camera -> position);
		model.second -> Uniform3f(model.second -> shader, "uniColor", model.second -> myColor);
		model.second -> Uniform1bool(model.second -> shader, "isSelected", model.second -> isSelected);
		model.second -> Draw(model.second -> shader);
	}
}
