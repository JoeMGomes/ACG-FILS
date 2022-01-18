#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;
out vec3 vertexPosition;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	gl_Position = MVP * vec4(pos, 1.0f);
	textureCoord = texCoord;
    vertexPosition = pos;
	fragPos = vec3(model * vec4(gl_Position.xyz, 1.0f));
    norm = normals; 
	norm = mat3(transpose(inverse(model)))*normals;
}