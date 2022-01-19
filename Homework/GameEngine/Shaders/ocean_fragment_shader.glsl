#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//Ambient light
	float ambientStrength = .7;
    vec3 ambient = ambientStrength * lightColor;

	//Diffuse light
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos); 

	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	//Specular light
	float specularStrength = .7;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 result = ambient + diffuse + specular;
//	vec3 result = ambient * vec3(1,0,0);
	fragColor = vec4(result*vec3(0,0.41,0.59), 1.0f);
}