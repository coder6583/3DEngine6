#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec3 ourColor;
out vec2 outTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(position.x, position.y, position.z, 1.0f);
	ourColor = vec3(0.0f, 0.0f, 0.0f);
	outTexCoords = texCoords;
}