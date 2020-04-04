#ifndef SQUARE_H
#define SQUARE_H

#include "Model.h"

class Square : public Model
{
public:
	Square(GLuint shaderProgram_, GLuint textureId_);
	virtual ~Square();
};

#endif // !SQUARE_H

