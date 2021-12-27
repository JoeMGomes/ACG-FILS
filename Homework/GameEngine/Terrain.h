#pragma once
#include "Model Loading/mesh.h"


class Terrain: public Mesh
{
	public:
		int width, height;
		Terrain(int width, int height);
};

