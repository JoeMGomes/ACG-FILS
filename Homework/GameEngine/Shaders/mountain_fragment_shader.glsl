#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;
in vec3 vertexPosition;


out vec4 fragColor;

uniform sampler2D dirtTex;
uniform sampler2D snowTex;
uniform sampler2D waterTex;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float maxHeight;



void main()
{
	vec3 dirtColor = texture(dirtTex, textureCoord).rgb;
	vec3 snowColor = texture(snowTex, textureCoord).rgb;
	vec3 waterColor = texture(waterTex, textureCoord).rgb;

	vec3 resultTex;

	float f = clamp(noise1(vertexPosition.xz*.05), 0.0,1.0)*10.8;
	f += noise1(vertexPosition.xz*.1+norm.yz*1.08)*.85;
	f *= .55;
	vec3 m = mix(vec3(.63*f+.2, .7*f+.1, .7*f+.1), vec3(f*.43+.1, f*.3+.2, f*.35+.1), f*.65);
	resultTex = m*vec3(f*m.x+.5, f*m.y+.5, f*m.z+.5);
//	resultTex = dirtColor;
	
	float ambientStrength = 1;
	float specularStrength = 0.0f;

	float snowLimit = 0.65f *maxHeight;


//	//normal color
		float v = norm.y;
		float c = (.5-v) * 4.0;
		c = clamp(c*c, 0.1, .2);
		f = noise1(vec2(vertexPosition.x*.09, vertexPosition.z*.095+vertexPosition.yy*0.15));
		f += noise1(vec2(vertexPosition.x*2.233, vertexPosition.z*2.23))*0.5;
		resultTex = mix(resultTex, vec3(.4*f), c);

	//grass
	if (vertexPosition.y < 0.45*maxHeight && norm.y > .35)
	{

		m = vec3(noise1(vertexPosition.xz*.023)*.5+.15, noise1(vertexPosition.xz*.03)*.6+.25, 0.0);
		m *= (norm.y- 0.65)*.6;
		resultTex = mix(resultTex, m, clamp((norm.y-.65)*1.3 * (45.35-vertexPosition.y)*0.1, 0.0, 1.0));
	}

	//snow
	if(vertexPosition.y > snowLimit && norm.y > 0.42){
		float aSnow = clamp((vertexPosition.y - snowLimit - noise1(vertexPosition.xz*.0001)*28.0),0,1.0f);
		resultTex = mix(resultTex, vec3(.7,.7,.8), aSnow);
		specularStrength += aSnow;
		ambientStrength += aSnow*.3f;
	}


	//Ambient and Diffuse light
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos); 
	float h = dot(normal, lightDir);

	float diffAmbi = max(h, 0.0f)+ambientStrength;
	resultTex = resultTex* diffAmbi * lightColor;

	//Specular light
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64)*specularStrength;
	resultTex = mix(resultTex, lightColor,spec); 

	fragColor = vec4(resultTex, 1.0f);
}