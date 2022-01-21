#include "collision.h"

BoundingBox::BoundingBox(const Mesh& mesh) {

	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	min_x = max_x = mesh.vertices[0].pos.x;
	min_y = max_y = mesh.vertices[0].pos.y;
	min_z = max_z = mesh.vertices[0].pos.z;
	for (int i = 0; i < mesh.vertices.size(); i++) {
		if (mesh.vertices[i].pos.x < min_x) min_x = mesh.vertices[i].pos.x;
		if (mesh.vertices[i].pos.x > max_x) max_x = mesh.vertices[i].pos.x;
		if (mesh.vertices[i].pos.y < min_y) min_y = mesh.vertices[i].pos.y;
		if (mesh.vertices[i].pos.y > max_y) max_y = mesh.vertices[i].pos.y;
		if (mesh.vertices[i].pos.z < min_z) min_z = mesh.vertices[i].pos.z;
		if (mesh.vertices[i].pos.z > max_z) max_z = mesh.vertices[i].pos.z;
	}

	
	this->center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	this->halfWidth = (max_x - min_x)/2.0f;
	this->halfHeight = (max_y - min_y) / 2.0f;
	this->halfDepth = (max_z - min_z) / 2.0f;

	this->initBBoxes();
}

void BoundingBox::initBBoxes() {

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void BoundingBox::draw() {

	glBindVertexArray(this->vao);

	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	//glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, 36*sizeof(float), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Counter::DrawCall();
}

bool BoundingBox::checkCollision(const BoundingBox& a, const BoundingBox& b) {
	return (a.center.x - a.halfWidth <= b.center.x + b.halfWidth && a.center.x + a.halfWidth >= b.center.x - b.halfWidth) &&
		(a.center.z - a.halfDepth <= b.center.z + b.halfDepth && a.center.z + a.halfDepth >= b.center.z - b.halfDepth) &&
		(a.center.y - a.halfHeight <= b.center.y + b.halfHeight && a.center.y + a.halfHeight >= b.center.y - b.halfHeight);
}


bool BoundingBox::checkCollision(const glm::vec3& a, const BoundingBox& b) {
	return (a.x <= b.center.x + b.halfWidth && a.x >= b.center.x - b.halfWidth) &&
		(a.z <= b.center.z + b.halfDepth && a.z >= b.center.z - b.halfDepth) &&
		(a.y <= b.center.y + b.halfHeight && a.y >= b.center.y - b.halfHeight);
}

