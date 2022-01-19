#version 400
#define DIRECTIONS_MAX_SIZE 30

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;
uniform float amplitude, frequency, q, uTime, phase, nWaves,directionsL;
uniform vec2 directions[DIRECTIONS_MAX_SIZE];


vec3 calculateP(){
	
	float sumX = pos.x;
	float sumY = pos.y;
	float sumZ = pos.z;

	vec3 sum = vec3(pos.x,pos.y,pos.z);

	for(int i=0; i<directionsL;i++){
		vec2 direction = normalize(directions[i]);
		float C = cos(dot(frequency*direction,pos.xz) + phase*uTime);
		float S = sin(dot(frequency*direction,pos.xz) + phase*uTime);
		float QA = q*amplitude;

		sum.x = QA * direction.x * C;
		sum.z = QA * direction.y * C;
		sum.y = amplitude * S;
	}

	return sum;
}

vec3 calculateNormals(in vec3 P){
	
	vec3 sum = vec3(0,1,0);
	for(int i=0; i<directionsL; i++){
		vec2 direction = normalize(directions[i]);
		float WA = frequency *amplitude;
		float S = sin(frequency*dot(direction,vec2(pos.x,pos.z))+phase*uTime);
		float C = cos(frequency*dot(direction,vec2(pos.x,pos.z))+phase*uTime);

		sum.x -= direction.x * WA * C;
		sum.z -= direction.y * WA * C;
		sum.y -= q*WA*S;

	}

	return normalize(sum);
}

void main()
{
	vec3 P = calculateP();
	vec3 position = vec3(pos.x, P.y, pos.z);
	textureCoord = texCoord;
	fragPos = vec3(model * vec4(position, 1.0f));
	norm = calculateNormals(P);
	gl_Position = MVP * vec4(position, 1.0f);
}