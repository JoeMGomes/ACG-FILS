#include "Terrain.h"
#include "main.h"
#include <random>
#include <algorithm>
#include "Model Loading/texture.h"

//Shader Terrain::mountainShader = Shader("Shaders/mountain_vertex_shader.glsl", "Shaders/mountain_fragment_shader.glsl");
SimplexNoise* TerrainChunk::noiseMaker = new SimplexNoise();

void TerrainChunk::loadTexture() {
	Texture snow, dirt, water;
	GLuint snowTex = loadBMP("Resources/Textures/snow.bmp");
	GLuint dirtTex = loadBMP("Resources/Textures/dirt.bmp");
	GLuint waterTex = loadBMP("Resources/Textures/water.bmp");

	snow.id = snowTex;
	dirt.id = dirtTex;
	water.id = waterTex;

	snow.type = "snowTex";
	dirt.type = "dirtTex";
	water.type = "waterTex";

	this->textures = { dirt, snow, water };
	
}


TerrainChunk::TerrainChunk(int width, int height, int startX, int startY, float maxHeight)
{
	this->seed = seed;
	this->maxHeight = maxHeight;
	this->width = width + 1;
	this->height = height + 1;
	this->loadTexture();
	this->startX = startX;
	this->startY = startY;
	this->zoom = 90.0f;
	auto lerp = [](float a, float b, float t) {  return (1 - t) * a + t * b; };
	auto smoothstep = [](float t,float low, float up) {	
		auto clamp = [](float t, float lower, float upper) {
			if (t < lower) t = lower;
			if (t > upper) t = upper;
			return t;
		};

		auto x = clamp((t - low) / (up - low), 0.0, 1.0);
		return x * x * (3 - 2 * x);
	};

	int index = 0;
	for (size_t y = 0; y < this->height; y++)
	{
		for (size_t x = 0; x < this->width; x++)
		{
			float vHeight = (noiseMaker->fractal(8, x/zoom + startX, y /zoom + startY)+1)/2;
			//float t = lerp(0, this->maxHeight, vHeight);
			//assert(t > 0 && t <= 1);
			float eastRatio = smoothstep(x,0.2*width,0.7*width);
			this->vertices.push_back(Vertex(x, vHeight*eastRatio*this->maxHeight, y, 0, 0, 0, x * 1.0f / (this->width - 1), 1.0f * y / (this->height - 1)));
			if (index < this->width * this->height - this->width - 1 && x < this->width - 1) {
				this->indices.push_back(index);
				this->indices.push_back(index+1);
				this->indices.push_back(index+this->width);

				this->indices.push_back(index+1);
				this->indices.push_back(index+1+this->width);
				this->indices.push_back(index+this->width);
			}
			index++;
		
		}	
	}
	this->calculateNormals();
	this->setup();
}

void TerrainChunk::calculateNormals() {
	for (size_t i = 0; i < this->indices.back() - this->width - 1; i++)
	{
		if (i % this->width == this->width - 1) continue;
		Vertex a, b, c, d;
		a = this->vertices[i];
		b = this->vertices[i + this->width];
		c = this->vertices[i + this->width + 1];
		d = this->vertices[i + 1];
		
		// A->B and B->C
		// B->C and C->D
		// C->D and D->A
		// D->A and A->B
		auto AB = b.pos - a.pos;
		auto BC = c.pos - b.pos;
		auto CD = d.pos - c.pos;
		auto DA = a.pos - d.pos;

		this->vertices[i].normals, 
		this->vertices[i + this->width].normals,
		this->vertices[i + this->width + 1].normals,
		this->vertices[i + 1].normals += glm::cross(AB, BC) + glm::cross(BC, CD) + glm::cross(CD, DA) + glm::cross(DA, AB);


	}

	for(auto& vertex : this->vertices)
	{
		if(glm::length(vertex.normals) > 1) vertex.normals = glm::normalize(vertex.normals);
		
	}

}

Terrain::Terrain(int rows, int cols, std::vector<Texture> texture, int chunkSize) 
{
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			this->chunks.push_back(TerrainChunk(32, 32, row * chunkSize - 1, cols * chunkSize - 1));
		}
	}
}
