#version 400

layout (location = 0) in vec3 pos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main(){

	vec4 transform = projection * view * vec4 (pos,1.0f);

	gl_Position = vec4(transform.x,transform.y, transform.w, transform.w);
	texCoords = vec3(pos.x,-pos.y,-pos.z);
}