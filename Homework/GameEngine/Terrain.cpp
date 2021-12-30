#include "Terrain.h"
#include "main.h"
#include <random>
#include <algorithm>

//Shader Terrain::mountainShader = Shader("Shaders/mountain_vertex_shader.glsl", "Shaders/mountain_fragment_shader.glsl");
SimplexNoise* TerrainChunk::noiseMaker = new SimplexNoise(500,500);

TerrainChunk::TerrainChunk(int width, int height, std::vector<Texture> texture, int startX, int startY, float maxHeight)
{
	this->seed = seed;
	this->maxHeight = maxHeight;
	this->width = width + 1;
	this->height = height + 1;
	this->textures = texture;
	this->scalingBias = 2.0f;
	this->startX = startX;
	this->startY = startY;
	
	int index = 0;
	for (size_t y = 0; y < this->height; y++)
	{
		for (size_t x = 0; x < this->width; x++)
		{
			this->vertices.push_back(Vertex(x, noiseMaker->noise(8, x + startX, y + startY), y, 0, 0, 0, 1.0f * y / (this->height - 1), x * 1.0f / (this->width - 1)));
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

	for (size_t i = 0; i < index - this->width - 1; i++)
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
		auto DA = d.pos - a.pos;

		this->vertices[i].normals, 
		this->vertices[i + this->width].normals,
		this->vertices[i + this->width + 1].normals,
		this->vertices[i + 1].normals += glm::cross(AB, BC) + glm::cross(BC, CD) + glm::cross(CD, DA) + glm::cross(DA, AB);
	}

	for(auto& vertex : this->vertices)
	{
		if(glm::length(vertex.normals) > 1) vertex.normals = glm::normalize(vertex.normals);
		
	}

	this->setup();
}

Terrain::Terrain(int rows, int cols, std::vector<Texture> texture, int chunkSize) 
{
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			this->chunks.push_back(TerrainChunk(32, 32, texture, row * chunkSize - 1, cols * chunkSize - 1));
		}
	}
}
