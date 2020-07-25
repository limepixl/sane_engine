#version 330 core
out vec4 color;
in vec2 texCoords;
in vec3 normal;
in vec3 FragPos;

uniform sampler2D tex;

void main()
{
	vec3 light = vec3(-5.0, 0.0, 0.0);
	float diffuse = dot(normal, normalize(light - FragPos));

	color = max(diffuse, 0.1) * texture(tex, texCoords);
}