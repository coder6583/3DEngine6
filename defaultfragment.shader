#version 330 core
out vec4 color;

in mat4 ourModel;
in mat4 ourView;
in vec3 ourPosition;

void main()
{
	vec3 ec_pos = (ourModel * ourView * vec4(ourPosition, 1.0f)).xyz;
	vec3 ec_normal = normalize(cross(dFdx(ec_pos), dFdy(ec_pos)));
	vec3 light_dir = vec3(0.0f, 0.85f, 0.85f);
	float d = dot(light_dir, ec_normal);
	color = vec4(1.0 * d, 1.0 * d, 1.0 * d, 0);
}