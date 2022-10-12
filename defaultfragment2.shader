#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 outNormal;
in vec3 outBC;
uniform vec3 lightPos;
uniform vec3 uniColor;
uniform bool isSelected;

void main()
{
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);
	vec3 result;
	if(isSelected)
	{
		if(outBC.x < 0.02f || outBC.y < 0.02f || outBC.z < 0.02f)
			result = vec3(1.0f, 1.0f, 1.0f);
		else
			result = (ambient + diffuse) * uniColor;
		result.x += 0.05f;
		result.y += 0.1f;
		result.z += 0.2f;
		if(result.x > 1.0f)
			result.x = 1.0f;
		if(result.y > 1.0f)
			result.y = 1.0f;
		if(result.z > 1.0f)
			result.z = 1.0f;
	}
	else if(!isSelected)
	{
		result = (ambient + diffuse) * uniColor;
	}
	color = vec4(result, 1.0);
}