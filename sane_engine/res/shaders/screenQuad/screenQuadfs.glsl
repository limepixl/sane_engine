#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D fbo;

void main()
{
	color = vec4(vec3(texture(fbo, texCoords)), 1.0);
}