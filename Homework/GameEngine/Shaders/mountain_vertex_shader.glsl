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


const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                      -0.80,  0.36, -0.48,
                      -0.60, -0.48,  0.64 );
const mat3 m3i = mat3( 0.00, -0.80, -0.60,
                       0.80,  0.36, -0.48,
                       0.60, -0.48,  0.64 );

void main()
{
	gl_Position = MVP * vec4(pos, 1.0f);
	textureCoord = texCoord;
	fragPos = vec3(model * vec4(gl_Position.xyz, 1.0f));
    norm = mat3(transpose(inverse(model)))*normals; 
}