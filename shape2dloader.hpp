#pragma once
#include "shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum class ShapeType{RECTANGLE, TRIANGLE, CIRCLE};

class Shape2DLoader
{
	public:
		Shape2DLoader();
		void LoadShape(ShapeType shapeType, glm::vec3 size = glm::vec3(0.3f, 0.5f, 0.0f));
		void ChangeSize(glm::vec3 scale);
		void Draw();
		~Shape2DLoader();
	private:
		GLuint VAOmember = 0;
		glm::vec3 size;
		glm::mat4 trans = glm::mat4(1.0f);
		Shader shader = Shader("2dshapevertex.shader", "2dshapefragment.shader");
};