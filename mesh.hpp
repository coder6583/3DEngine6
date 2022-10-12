#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	//
	glm::vec3 barycentric;
	//
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
		void Draw(Shader shader);
	private:
		GLuint VAO, VBO, EBO;
		
		void setUpMesh();
};