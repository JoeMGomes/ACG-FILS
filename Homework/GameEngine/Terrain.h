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
		float zoom;
		static SimplexNoise* noiseMaker;
		TerrainChunk(int width, int height,int startX=0, int startY =0, float maxHeight = 100);
		void calculateNormals();
private:
	float seed;
	void loadTexture();
};

class Terrain{
private:
	std::vector<TerrainChunk> chunks;

public:
	Terrain(int rows, int cols, std::vector<Texture> texture, int chunkSize = 32);
};