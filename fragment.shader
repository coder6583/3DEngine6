#version 330 core
in vec3 ourColor;
in vec2 outTexCoords;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	color = mix(texture(ourTexture1, outTexCoords), texture(ourTexture2, vec2(-outTexCoords.x, -outTexCoords.y)), 0.2);
}