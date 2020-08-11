#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 texCoords;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 nonTranslatedView;

void main()
{
	texCoords = aPos;
	gl_Position = (projection * nonTranslatedView * vec4(aPos, 1.0)).xyww; 
}