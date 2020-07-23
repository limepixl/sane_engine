#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;
out vec3 normal;
out vec3 pos;

void main()
{
	texCoords = aTexCoords;
	normal = (view * model * vec4(aNormal, 1.0)).xyz;
	pos = (view * model * vec4(aPos, 1.0)).xyz;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}