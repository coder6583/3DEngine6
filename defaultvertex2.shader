#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 barycentric;

out vec3 outNormal;
out vec3 FragPos;
out vec3 outBC;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outNormal = normal;
	outBC = barycentric;
	FragPos = vec3(model * vec4(position, 1.0f));
	gl_Position = projection * view * model * vec4(position, 1.0f);
}