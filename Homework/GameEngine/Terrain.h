#pragma once
#include "Model Loading/mesh.h"
#include "Shaders/shader.h"
#include "Utils/SimplexNoise.h"




class TerrainChunk: public Mesh
{
	public:
		int width, height;
		int startX, startY;
		float maxHeight;
		static Shader mountainShader;
		float* noise;
		float scalingBias;
		static SimplexNoise* noiseMaker;
		TerrainChunk(int width, int height, std::vector<Texture> texture,int startX=0, int startY =0, float maxHeight = 15);
private:
	float seed;
};

class Terrain{
private:
	std::vector<TerrainChunk> chunks;

public:
	Terrain(int rows, int cols, std::vector<Texture> texture, int chunkSize = 32);
	void draw(Shader shader);
};