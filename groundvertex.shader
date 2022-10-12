#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 outNormal;
out vec3 FragPos;
out vec2 outTexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outNormal = normal;
	FragPos = vec3(model * vec4(position, 1.0f));
	outTexCoords = vec2( 512 * texCoords.x, 512 * texCoords.y);
	gl_Position = projection * view * model * vec4(position, 1.0f);
}