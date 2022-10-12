#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 outNormal;
in vec2 outTexCoords;
uniform sampler2D grid;
uniform vec3 lightPos;

void main()
{
	float ambientStrength = 1.0f;
	vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);
	vec3 result;
	result = (ambient + diffuse) * vec3(texture(grid, outTexCoords));
	color = vec4(result, 0.0);
}