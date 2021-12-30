#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

out vec2 textureCoord; 
out vec3 norm;
out vec3 fragPos;

uniform mat4 projection;

in DATA{
	vec3 norm;
	vec3 fragPos;
	vec2 textureCoord;
} data_in[];

void main(){
	gl_Position = projection * gl_in[0].gl_Position;
	norm = data_in[0].norm;
	textureCoord = data_in[0].textureCoord;
	fragPos = data_in[0].fragPos;
	EmitVertex();
	gl_Position = projection * gl_in[1].gl_Position;
	norm = data_in[1].norm;
	textureCoord = data_in[1].textureCoord;
	fragPos = data_in[1].fragPos;
	EmitVertex();
	gl_Position = projection * gl_in[2].gl_Position;
	norm = data_in[2].norm;
	textureCoord = data_in[2].textureCoord;
	fragPos = data_in[2].fragPos;
	EmitVertex();
}