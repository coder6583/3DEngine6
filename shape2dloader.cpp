#include "shape2dloader.hpp"

Shape2DLoader::Shape2DLoader()
{
	glGenVertexArrays(1, &this -> VAOmember);
}
Shape2DLoader::~Shape2DLoader()
{
		glDeleteVertexArrays(1, &this-> VAOmember);
}

void Shape2DLoader::LoadShape(ShapeType shapeType, glm::vec3 size)
{
	this -> size = size;
	GLuint VBO;
	GLuint EBO;
	glGenVertexArrays(1, &this -> VAOmember);

	glGenBuffers(1, &VBO);
	
	glBindVertexArray(this -> VAOmember);
	
	if(shapeType == ShapeType::RECTANGLE)
	{
		glGenBuffers(1, &EBO);
		GLfloat vertices[] = 
		{
			this -> size.x, this -> size.y, 0.0f,
			this -> size.x, -this -> size.y, 0.0f,
			-this -> size.x, -this -> size.y, 0.0f,
			-this -> size.x, this -> size.y, 0.0f
		};
		GLint indices[] = 
		{
			0, 1, 3,
			1, 2, 3
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		
		glBindVertexArray(0);
	}
	// 	this -> VAOmember = VAO;

}

void Shape2DLoader::ChangeSize(glm::vec3 scale)
{
	this -> trans = glm::scale(this -> trans, scale);
	glUniformMatrix4fv(glGetUniformLocation(this -> shader.Program, "trans"), 1, GL_FALSE, glm::value_ptr(this -> trans));
}

void Shape2DLoader::Draw()
{
	this -> shader.Use();
	glBindVertexArray(this -> VAOmember);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
}















