#include "headers/MatrixStack.h"

void MatrixStack::pushMatrix(const vmml::Matrix4f &matrix)
{
	_matrixStack.push_back(matrix);
}

void MatrixStack::popMatrix()
{
	_matrixStack.pop_back();
}

void MatrixStack::clearMatrixStack()
{
	_matrixStack.clear();
}


vmml::Matrix4f MatrixStack::getMatrix() 
{
    vmml::Matrix4f transformationMatrix = vmml::Matrix4f::IDENTITY;
    
    // multiply all transformations on the Stack to get the final transformation matrix
	for (std::vector<vmml::Matrix4f>::reverse_iterator it = _matrixStack.rbegin(); it != _matrixStack.rend(); ++it)
    {
		transformationMatrix = transformationMatrix * *it;
    }
        
    return transformationMatrix;
}
