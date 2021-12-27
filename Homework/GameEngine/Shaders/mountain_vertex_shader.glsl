#version 400

#ifdef GL_ES
precision mediump float;
#endif

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

float random(in vec2 st){
	return fract(sin(dot(st.xy,vec2(23.7836452,98.23412)))*3467.9238470);
}

void main()
{
	gl_Position = MVP * vec4(pos, 1.0f);
	vec2 st = vec2(gl_Position.x, gl_Position.z);
//	gl_Position.y += random(st);
	textureCoord = texCoord;
	fragPos = vec3(model * vec4(gl_Position.xyz, 1.0f));
	norm = mat3(transpose(inverse(model)))*normals;
}