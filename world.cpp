#include "world.hpp"

World::World(btVector3 gravity)
{
	this -> collisionConfiguration = new btDefaultCollisionConfiguration();
	this -> dispatcher = new btCollisionDispatcher(this -> collisionConfiguration);
	this -> overlappingPairCache = new btDbvtBroadphase();
	this -> solver = new btSequentialImpulseConstraintSolver;
	this -> dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	this -> dynamicsWorld -> setGravity(gravity);
	
	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(1024.), btScalar(50.), btScalar(1024.)));
	btTransform groundTransform;
	this -> collisionShapes.push_back(groundShape);
	
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));
	btScalar mass(0.);
	
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if(isDynamic)
		groundShape -> calculateLocalInertia(mass, localInertia);
		
	btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);
	
	dynamicsWorld -> addRigidBody(body);
	int imageWidth, imageHeight;
	unsigned char *image = SOIL_load_image("grid.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	SOIL_free_image_data(image);

	std::shared_ptr<Model> groundModel = std::make_shared<Model>(Shape3DType::GROUND, glm::vec3(1024.0f, 0.0f, 1024.0f));
	this -> loadedModels.insert(std::make_pair(groundModel -> GetName(), groundModel));
	this -> loadedModels.at(groundModel -> GetName()) -> shader = this -> groundShader;
}

World::World(std::string filename)
{
	this -> worldFilePath = filename;
	std::ifstream worldFile(filename);
	this -> gravity = btVector3(0, 0, 0);
	float gx, gy, gz;
	int objNum;
	std::string line;
	std::istringstream ssline;
	getline(worldFile, line);
	ssline = std::istringstream(line);
	ssline >> line >> gx >> gy >> gz;
	gravity.setX(gx);
	gravity.setY(gy);
	gravity.setZ(gz);
	getline(worldFile, line);
	ssline = std::istringstream(line);
	ssline >> line >> objNum;
	
	this -> collisionConfiguration = new btDefaultCollisionConfiguration();
	this -> dispatcher = new btCollisionDispatcher(this -> collisionConfiguration);
	this -> overlappingPairCache = new btDbvtBroadphase();
	this -> solver = new btSequentialImpulseConstraintSolver;
	this -> dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	this -> dynamicsWorld -> setGravity(gravity);
	
	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(1024.), btScalar(50.), btScalar(1024.)));
	btTransform groundTransform;
	this -> collisionShapes.push_back(groundShape);
	
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));
	btScalar mass(0.);
	
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if(isDynamic)
		groundShape -> calculateLocalInertia(mass, localInertia);
		
	btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);
	
	dynamicsWorld -> addRigidBody(body);
	int imageWidth, imageHeight;
	unsigned char *image = SOIL_load_image("grid.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	SOIL_free_image_data(image);

	std::shared_ptr<Model> groundModel = std::make_shared<Model>(Shape3DType::GROUND, glm::vec3(1024.0f, 0.0f, 1024.0f));
	this -> loadedModels.insert(std::make_pair(groundModel -> GetName(), groundModel));
	this -> loadedModels.at(groundModel -> GetName()) -> shader = this -> groundShader;
	
	std::string objName, objPath, objType;
	float posx, posy, posz, objMass, axisx, axisy, axisz, rad;
	float objSize; 
	float color[3];
	for(int i = 0; i < objNum; i++)
	{
		getline(worldFile, objName, ':');
		getline(worldFile, line);
		ssline = std::istringstream(line);
		ssline >> objPath >> posx >> posy >> posz >> color[0] >> color[1] >> color[2] >> objMass >> axisx >> axisy >> axisz >> rad >> objType;
		std::cout << objType << std::endl;
		if(objType == "CUBE")
		{
			ssline >> objSize;
			this -> AddCube(objSize, glm::vec3(posx, posy, posz), objMass, color, btVector3(axisx, axisy, axisz), rad);
		}
		else if(objType == "MODEL")
		{
			this -> AddModel(objPath, glm::vec3(posx, posy, posz), objMass, color, btVector3(axisx, axisy, axisz), rad);
		}
	}
	worldFile.close();
}

World::~World()
{
	for(int i = this -> dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	for(int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}
	delete this -> dynamicsWorld;
	delete this -> solver;
	delete this -> overlappingPairCache;
	delete this -> dispatcher;
	delete this -> collisionConfiguration;
}

bool World::AddModel(std::string path, glm::vec3 pos, float mass, float color[3], btVector3 axis, float radians)
{
	std::shared_ptr<Model> newModel = std::make_shared<Model>(Shape3DType::MODEL, path, pos, mass, color);
	if(this -> loadedModels.find(newModel -> GetName()) == this -> loadedModels.end())
	{
		this -> loadedModels.insert(std::make_pair(newModel -> GetName(), newModel));

		if(newModel -> HasTexture())
			this -> loadedModels.at(newModel -> GetName()) -> shader = this -> shader;
		else if(!newModel -> HasTexture())
			this -> loadedModels.at(newModel -> GetName()) -> shader = this -> defaultShader;
	}

	else if(this -> loadedModels.find(newModel -> GetName()) != this -> loadedModels.end())
	{
		int i = 1;

		while(this -> loadedModels.find(newModel -> GetName()) != this -> loadedModels.end())
		{
			newModel -> NameToOriginal();
			newModel -> ChangeName(i);
			i++;
		}
		this -> loadedModels.insert(std::make_pair(newModel -> GetName(), newModel));
		if(newModel -> HasTexture())
		{
			this -> loadedModels.at(newModel -> GetName()) -> shader = this -> shader;
		}
		else if(!newModel -> HasTexture())
		{
			this -> loadedModels.at(newModel -> GetName()) -> shader = this -> defaultShader;
		}

	}
	v_names.push_back(newModel -> GetName());
	collisionShapes.push_back(newModel -> colShape);
	this -> dynamicsWorld -> addRigidBody(newModel -> body);
	this -> RotateModel(newModel -> GetName(), axis, radians);
	return true;
}

bool World::AddCube(float size, glm::vec3 pos, float mass, float color[3], btVector3 axis, float radians)
{
	std::shared_ptr<Model> newModel = std::make_shared<Model>(Shape3DType::CUBE, size, pos, mass, color);
	if(this -> loadedModels.find(newModel -> GetName()) == this -> loadedModels.end())
	{
		this -> loadedModels.insert(std::make_pair(newModel -> GetName(), newModel));
		this -> loadedModels.at(newModel -> GetName()) -> shader = this -> defaultShader;
// 		this -> m_shaderMap.insert(std::make_pair(newModel -> GetName(), this -> defaultShader));
	}

	else if(this -> loadedModels.find(newModel -> GetName()) != this -> loadedModels.end())
	{
		int i = 1;

		while(this -> loadedModels.find(newModel -> GetName()) != this -> loadedModels.end())
		{
			newModel -> NameToOriginal();
			newModel -> ChangeName(i);
			i++;
		}
		this -> loadedModels.insert(std::make_pair(newModel -> GetName(), newModel));
		this -> loadedModels.at(newModel -> GetName()) -> shader = this -> defaultShader;
	}

	collisionShapes.push_back(newModel -> colShape);

	this -> dynamicsWorld->addRigidBody(newModel -> body);
	v_names.push_back(newModel -> GetName());
	this -> RotateModel(newModel -> GetName(), axis, radians);
	return true;
}

bool World::SaveWorld()
{
	std::ofstream worldFile;
	std::cout << this -> worldFilePath << std::endl;
	worldFile.open(this -> worldFilePath, std::ofstream::out | std::ofstream::trunc);
	btVector3 gravity = this -> dynamicsWorld -> getGravity();
	worldFile << "gravity: " << this -> gravity.getX() << " " << this -> gravity.getY() << " " << this -> gravity.getZ() << " \n";
	worldFile << "number: " << this -> loadedModels.size() - 1 << " \n";
	for(int i =0; i < this -> v_names.size(); i++)
	{
		std::shared_ptr<Model> newModel = this -> loadedModels.at(this -> v_names.at(i));
		worldFile << this -> v_names.at(i) << ": ";
		if(newModel -> GetType() == Shape3DType::CUBE)
		{
			worldFile << "none " << newModel -> body -> getCenterOfMassPosition().getX() << " " << newModel -> body -> getCenterOfMassPosition().getY() << " " << newModel -> body -> getCenterOfMassPosition().getZ() << " " << newModel -> myColor.x << " " << newModel -> myColor.y << " " << newModel -> myColor.z << " " << newModel -> mass << " " << newModel -> GetTransform() -> getRotation().getAxis().getX() << " " << newModel -> GetTransform() -> getRotation().getAxis().getY() << " " << newModel -> GetTransform() -> getRotation().getAxis().getZ() << " " << newModel -> GetTransform() -> getRotation().getAngle() << " CUBE " << newModel -> GetSize() << " \n";
		}
		if(newModel -> GetType() == Shape3DType::MODEL)
		{
			worldFile << newModel -> GetPath() << " " << newModel -> body -> getCenterOfMassPosition().getX() << " " << newModel -> body -> getCenterOfMassPosition().getY() << " " << newModel -> body -> getCenterOfMassPosition().getZ() << " " << newModel -> myColor.x << " " << newModel -> myColor.y << " " << newModel -> myColor.z << " " << newModel -> mass << " " << newModel -> GetTransform() -> getRotation().getAxis().getX() << " " << newModel -> GetTransform() -> getRotation().getAxis().getY() << " " << newModel -> GetTransform() -> getRotation().getAxis().getZ() << " " << newModel -> GetTransform() -> getRotation().getAngle() << " MODEL " << " \n";
		}
	}
	worldFile.close();
	return true;
}

bool World::RemoveBody(std::string name)
{
	auto subject = std::find(this -> v_names.begin(), this -> v_names.end(), name);
	v_names.erase(subject);
	this -> dynamicsWorld -> removeRigidBody(this -> loadedModels.at(name) -> body);
	return true;
}

bool World::AddBody(std::string name)
{
	v_names.push_back(name);
	this -> dynamicsWorld -> addRigidBody(this -> loadedModels.at(name) -> body);
	return true;
}

bool World::RotateModel(std::string name, btVector3 axis, float angle)
{
	this -> RemoveBody(name);
	this -> loadedModels.at(name) -> Rotate(axis, angle);
	this -> AddBody(name);
}




















