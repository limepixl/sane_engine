#version 330 core
out vec4 color;
in vec2 texCoords;
in vec3 normal;
in vec3 FragPos;

uniform sampler2D diffusetex;
uniform sampler2D speculartex;

uniform vec3 cameraPos;
uniform vec3 lightPos;

void main()
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectedDir = reflect(-lightDir, normal);

	float diff = max(0.1, dot(normal, lightDir));
	vec3 diffuse = texture(diffusetex, texCoords).xyz * diff;

	vec3 viewDir = normalize(cameraPos - FragPos);
	float spec = max(0.0, dot(viewDir, reflectedDir));
	float specular = texture(speculartex, texCoords).b * spec;

	color = vec4(diffuse * (1.0 + 3.0*specular), 1.0);
}