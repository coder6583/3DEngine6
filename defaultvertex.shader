#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat4 ourModel;
out mat4 ourView;
out vec3 ourPosition;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
}