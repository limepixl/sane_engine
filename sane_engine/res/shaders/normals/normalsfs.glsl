#version 330 core
out vec4 color;
in vec2 texCoords;
in vec3 normal;
in vec3 FragPos;

uniform sampler2D diffusetex;
uniform sampler2D speculartex;

uniform vec3 cameraPos;

#define MAX_LIGHTS 10
#define LIGHT_DISTANCE 1.0 / 8.0
uniform vec3 lightPositions[MAX_LIGHTS] = vec3[MAX_LIGHTS]
(
	vec3(1000.0), vec3(1000.0), vec3(1000.0), vec3(1000.0), 
	vec3(1000.0), vec3(1000.0), vec3(1000.0), vec3(1000.0), 
	vec3(1000.0), vec3(1000.0)
);

vec3 CalcLight(vec3 lightPos, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	float dist = length(lightPos - FragPos) * LIGHT_DISTANCE;
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectedDir = reflect(-lightDir, normal);

	float diff = max(0.1, dot(normal, lightDir));
	vec3 diffuse = texture(diffusetex, texCoords).xyz * diff;

	float spec = max(0.0, dot(viewDir, reflectedDir));
	float specular = texture(speculartex, texCoords).b * spec;

	return max(0.0, 1 - dist) * diffuse * (1.0 + specular);
}

void main()
{
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 result = vec3(0.0);
	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		// This might be more expensive than checking all MAX_LIGHTS number of lights
		vec3 current = lightPositions[i];
		if(current.x == current.y && current.y == current.z && abs(current.x - 1000.0) < 0.1)
			break;

		result += CalcLight(lightPositions[i], normal, FragPos, viewDir);
	}

	color = vec4(result, 1.0);
}