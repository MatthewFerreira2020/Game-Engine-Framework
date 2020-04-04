#include "Square.h"



Square::Square(GLuint shaderProgram_, GLuint textureId_) : Model( shaderProgram_ )
{
	Vertex v;
	std::vector<Vertex> vertexList;

	//TRIANGLE ONE
	//pos1
	v.position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertexList.push_back(v);
	//pos2
	v.position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertexList.push_back(v);
	//pos3
	v.position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertexList.push_back(v);
	
	//TRIANGLE TWO
	//pos1
	v.position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertexList.push_back(v);
	//pos2
	v.position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertexList.push_back(v);
	//pos3
	v.position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertexList.push_back(v);

	//pass addmesh the address of a vertex list
	// now also takes in a shaderProgram
	AddMesh(new Mesh(&vertexList, textureId_, shaderProgram));
}


Square::~Square()
{
}
