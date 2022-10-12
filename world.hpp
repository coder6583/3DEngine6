#include "model.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

class World
{
	public:
		World(btVector3 gravity = btVector3(0, -9.81, 0));
		World(std::string filename);
		~World();
		bool AddModel(std::string path, glm::vec3 pos, float mass, float color[3], btVector3 axis = btVector3(0, 0, 1), float radians = 0);
		bool AddCube(float size, glm::vec3 pos, float mass, float color[3], btVector3 axis = btVector3(0, 0, 1), float radians = 0);
		bool SaveWorld();
		bool UpdateWorld();
		bool RemoveBody(std::string name);
		bool AddBody(std::string name);
		bool RotateModel(std::string name, btVector3 axis, float angle);
		btDiscreteDynamicsWorld* getWorld() const
		{
			return dynamicsWorld;
		}
		btAlignedObjectArray<btCollisionShape *> getColShape() const
		{
			return collisionShapes;
		}
		std::map<std::string, std::shared_ptr<Model>> getModels() const
		{
			return loadedModels;
		}
		std::deque<std::string> getNames() const
		{
			return v_names;
		}
	private:
		Shader shader = Shader("vertex2.shader", "fragment4.shader");
		Shader defaultShader = Shader("defaultvertex2.shader", "defaultfragment2.shader");
		Shader groundShader = Shader("groundvertex.shader", "groundfragment.shader");
		btDiscreteDynamicsWorld *dynamicsWorld;
		btDefaultCollisionConfiguration *collisionConfiguration;
		btCollisionDispatcher *dispatcher;
		btBroadphaseInterface *overlappingPairCache;
		btSequentialImpulseConstraintSolver *solver;
		btAlignedObjectArray<btCollisionShape *> collisionShapes;
		std::deque<std::string> v_names;
		std::map<std::string, std::shared_ptr<Model>> loadedModels;
		btVector3 gravity;
		std::string worldFilePath;
};