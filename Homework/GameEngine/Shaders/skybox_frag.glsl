#version 400

in vec3 texCoords;

out vec4 fragColor;

uniform samplerCube skyTexture;

void main()
{
    fragColor = texture(skyTexture, texCoords);
}