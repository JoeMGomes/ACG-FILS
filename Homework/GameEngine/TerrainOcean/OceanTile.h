#pragma once
#include <glm.hpp>
#include "../Model Loading/mesh.h"

class OceanTile: public Mesh
{
public:

	typedef std::vector<glm::vec2> Directions;

	OceanTile(int width,int height,
		Directions directions = { glm::vec2(1,0),glm::vec2(-0.5,-0.2) },
			  float amplitude = 1.26,
			  float waveLength = 4.94,float nWaves = 2,float speed = 7);
	void draw(Shader shader);
	void setDirections(Directions directions);
	void calculateConstants();

	float frequency, waveLength, nWaves, speed,
		amplitude,
		q, // Wave stepness
		phase; // speed in phase
	int width, height;
	std::vector<float> direction;

	void loadTexture();
};

