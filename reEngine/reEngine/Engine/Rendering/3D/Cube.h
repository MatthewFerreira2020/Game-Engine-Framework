#ifndef CUBE_H
#define CUBE_H

#include "Model.h"

class Cube : public Model
{
public:
	Cube(GLuint shaderProgram, GLuint textureId_);
	virtual ~Cube();
};

#endif // CUBE_H