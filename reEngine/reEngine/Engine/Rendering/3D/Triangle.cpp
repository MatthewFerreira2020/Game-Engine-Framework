#include "Triangle.h"



Triangle::Triangle(GLuint shaderProgram_, GLuint textureId_) : Model(shaderProgram_)
{
	Vertex v;
	std::vector<Vertex> vertexList;

	//pos1
	v.position = glm::vec3(0.0f, 0.5f, 0.0f);
	v.colour = glm::vec3(1.0f, 0.0f, 0.0f);
	vertexList.push_back(v);
	//pos2
	v.position = glm::vec3(-0.5f, -0.5f, 0.0f);
	v.colour = glm::vec3(0.0f, 1.0f, 0.0f);
	vertexList.push_back(v);
	//pos3
	v.position = glm::vec3(0.5f, -0.5f, 0.0f);
	v.colour = glm::vec3(0.0f, 0.0f, 1.0f);
	vertexList.push_back(v);

	//pass addmesh the address of a vertex list
	//now must also take in shader program
	AddMesh(new Mesh(&vertexList, textureId_, shaderProgram));
}


Triangle::~Triangle()
{
}
