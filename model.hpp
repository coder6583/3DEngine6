#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <sstream>
#include <deque>
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include "LoadMeshFromSTL.h"
#include "GLInstanceGraphicsShape.h"
#include "b3BulletDefaultFileIO.h"

#include "SOIL.h"
#include "interpreter.hpp"

enum class Shape3DType {MODEL, CUBE, CYLINDER, SPHERE, TRIPYRAMID, PYRAMID, GROUND, TRIPRISM};

GLint TextureFromFile(const char *path, std::string directory);


class Model
{
	public:
		Model(Shape3DType Type3D)
		{
			this -> Type3D = Type3D;
		}
		Model(Shape3DType Type3D, std::string path, glm::vec3 pos, float mass, float color[3])
		{
			this -> Type3D = Type3D;
		
			this -> firstPos = pos;
			this -> myColor.x = color[0];
			this -> myColor.y = color[1];
			this -> myColor.z = color[2];
			this -> mass = btScalar(mass);
			this -> loadModel(path);
			this -> SetOriginalTransform();
			this -> path = path;
			this -> isSelected = false;
		}
		Model(Shape3DType Type3D, float size, glm::vec3 pos, float mass, float color[3]);
		Model(Shape3DType Type3D, glm::vec3 size);
		void Draw(Shader shader);
		std::string GetPath() const
		{
			return this -> path;
		}
		float GetSize() const
		{
			return this -> size;
		}
		Shape3DType GetType() const
		{
			return this -> Type3D;
		}
		glm::vec3 GetMax() const
		{
			return max;
		}
		bool HasTexture() const
		{
			return this -> hasTexture;
		}
		std::string GetName() const
		{
			return this -> name;
		}
		void ChangeName(int i)
		{
			this -> name.push_back('(');
			this -> name.append(std::to_string(i));
			this -> name.push_back(')');
		}
		void NameToOriginal()
		{
			this -> name = this -> original;
		}
		void ChangeModelMat(glm::mat4 newModelMat)
		{
			this -> model = newModelMat;
		}
		void SetToOriginalTransform()
		{
			if(this -> Type3D != Shape3DType::GROUND)
			{
				this -> body -> getMotionState() -> setWorldTransform(this -> originalTrans);
			}
		}
		btTransform GetOriginalTransform()
		{
			return this -> originalTrans;
		}
		void resetBody()
		{
			this -> body = new btRigidBody(*originalInfo);
		}
		void SetOriginalTransform()
		{
			this -> body -> getMotionState() -> getWorldTransform(this -> originalTrans);
		}
		void resetInertia()
		{
			this -> localInertia = btVector3(0, 0,0);
		}
		btTransform* GetTransform()
		{
			this -> body -> getMotionState() -> getWorldTransform(this -> trans);
			return &this -> trans;
		}
		void MVPUniform(Shader shader, Camera camera, int width, int height);
		void UniformMat4(Shader shader, std::string uniName, glm::mat4 matrix);
		void Uniform3f(Shader shader, std::string uniName, glm::vec3 vector);
		void Uniform1bool(Shader shader, std::string uniName, bool i);
		std::string GetModelPath() {return this -> path;}
		glm::mat4 GetModel() {return this -> model;}
		void MoveTo(glm::vec3 nextPos);
		void Move(glm::vec3 offset);
		void Rotate(btVector3 axis, btScalar angle);
		void Scale(glm::vec3 scaleRatio);
		void Run();
		
		~Model()
		{
			
		}
		bool isRunning = false;
		bool isSelected = false;
		char sourceCode[1024 * 16] = "";
		Interpreter interpreter;
		Shader shader;
		commandVector loopCommands;
		glm::vec3 currentPos = glm::vec3(0.0f, 0.0f, 0.0f);
		btRigidBody *body;
		btCollisionShape *colShape;
		btScalar mass;
		btVector3 localInertia;
		glm::vec3 myColor;

	private:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory, path, name, original;
		glm::vec3 max = glm::vec3(0.0f, 0.0f, 8.0f);
		bool hasTexture = false;
		Shape3DType Type3D;
		GLuint VAO, VBO, groundTex;
		glm::vec3 firstPos = glm::vec3(0.0f);
		
		float size = 1.0f;
		
		bool isDynamic = false;
		btTransform trans;
		btDefaultMotionState *myMotionState;
		
		btConvexHullShape *modelShape;
		btRigidBody::btRigidBodyConstructionInfo *originalInfo;
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		btTransform originalTrans;
				
		void loadModel(std::string path);
		void loadCube(float size, glm::vec3 pos, float mass, float color[3]);
		void loadGround(glm::vec3 size);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
