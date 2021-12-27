#include "Terrain.h"

Terrain::Terrain(int width, int height)
{
	this->width = width;
	this->height = height;

	int index = 0;
	for (size_t i = 0; i < height + 1; i++)
	{
		for (size_t j = 0; j < width + 1; j++)
		{
			this->vertices.push_back(Vertex(i, 0, j, 0, 1, 0));
			if (j < width && i < height) {
				this->indices.push_back(index);
				this->indices.push_back(index+1);
				this->indices.push_back(index+width + 1);

				this->indices.push_back(index+1);
				this->indices.push_back(index+1+width + 1);
				this->indices.push_back(index+width + 1);
			}
			index++;
		
		}

		
	}

	this->setup2();
}
