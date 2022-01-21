#include "OceanTile.h"
#include "../Model Loading/texture.h"

static constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841;

OceanTile::OceanTile(int width, int height, Directions directions, float amplitude, float waveLength, float nWaves, float speed) {
	this->width = width;
	this->height = height;
	this->amplitude = amplitude;
	this->waveLength = waveLength;
	this->nWaves = nWaves;
	this->speed = speed;
	this->q = 0.5;
	this->calculateConstants();

	this->setDirections(directions);

	int index = 0;
	for (size_t y = 0; y < this->height; y++)
	{
		for (size_t x = 0; x < this->width; x++)
		{

			this->vertices.push_back(Vertex(x, 0, y, 0, 0, 0, x * 1.0f / (this->width - 1), 1.0f * y / (this->height - 1)));
			if (index < this->width * this->height - this->width - 1 && x < this->width - 1) {
				this->indices.push_back(index);
				this->indices.push_back(index + 1);
				this->indices.push_back(index + this->width);

				this->indices.push_back(index + 1);
				this->indices.push_back(index + 1 + this->width);
				this->indices.push_back(index + this->width);
			}
			index++;

		}
	}
	this->setup();
}

void OceanTile::calculateConstants() {
	this->frequency = sqrt(9.8 * 2 * PI / ((double)waveLength * width));

	//this->q = q / (this->frequency * this->amplitude * nWaves);

	this->phase = speed * width * 1.0f / (2 * ((double)waveLength * width));
}

void OceanTile::draw(Shader shader)
{
	glUniform1f(glGetUniformLocation(shader.getId(), "amplitude"), this->amplitude);
	glUniform1f(glGetUniformLocation(shader.getId(), "frequency"), this->frequency);
	glUniform1f(glGetUniformLocation(shader.getId(), "q"), this->q);
	glUniform1f(glGetUniformLocation(shader.getId(), "phase"), this->phase);
	glUniform1f(glGetUniformLocation(shader.getId(), "directionsL"), this->direction.size()/2);
	glUniform2fv(glGetUniformLocation(shader.getId(), "directions"), this->direction.size(), this->direction.data());

	Mesh::draw(shader);
}

void OceanTile::setDirections(Directions directions)
{
	this->direction.clear();

	for (auto direction : directions) {
		this->direction.push_back(direction.x);
		this->direction.push_back(direction.y);
	}
}

void OceanTile::loadTexture() {
	Texture water;
	GLuint waterTex = loadBMP("Resources/Textures/water.bmp");
	water.id = waterTex;
	water.type = "waterTex";

	this->textures = { water };

}