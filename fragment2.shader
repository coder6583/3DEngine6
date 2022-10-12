#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutoff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};
out vec4 color;

in vec3 outNormal;
in vec3 fragPos;
in vec2 outTexCoords;

uniform Material material;
uniform Light light;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	vec3 lightDir = normalize(lightPos - fragPos);
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	
	vec3 result;
	
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float ambientStrength = 0.2f;
	vec3 ambient = vec3(texture(material.diffuse, outTexCoords)) * light.ambient;
	
	vec3 norm = normalize(outNormal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, outTexCoords)) * light.diffuse;
	
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, outTexCoords).rgb;
	
	diffuse *= intensity;
	specular *= intensity;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
		
	
	result = ambient + diffuse + specular; 
	
	color = vec4(result, 1.0f);
}