#version 330 core
out vec4 color;
in vec2 texCoords;
in vec3 normal;
in vec3 pos;

uniform sampler2D tex;

void main()
{
	vec3 light = vec3(5.0, 10.0, -6.0);
	float diffuse = dot(normal, normalize(light - pos));

	color = max(diffuse, 0.1) * texture(tex, texCoords);
}