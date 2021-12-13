#pragma once
#include <glm.hpp>

struct BoundingBox
{

	glm::vec3 center;
	float halfWidth, halfHeight, halfDepth;

	static bool checkCollision(const BoundingBox& a , const BoundingBox& b) {
		return (a.center.x - b.halfWidth  <= b.center.x + b.halfWidth  && a.center.x + b.halfWidth  >= b.center.x - b.halfWidth) &&
			   (a.center.z - b.halfDepth  <= b.center.z + b.halfDepth  && a.center.z + b.halfDepth  >= b.center.z - b.halfDepth) &&
			   (a.center.y - b.halfHeight <= b.center.y + b.halfHeight && a.center.y + b.halfHeight >= b.center.y - b.halfHeight);
	}


	static bool checkCollision(const glm::vec3& a, const BoundingBox& b) {
		return (a.x <= b.center.x + b.halfWidth  && a.x  >= b.center.x - b.halfWidth) &&
			   (a.z <= b.center.z + b.halfDepth  && a.z  >= b.center.z - b.halfDepth) &&
			   (a.y <= b.center.y + b.halfHeight && a.y  >= b.center.y - b.halfHeight);
	}
};

