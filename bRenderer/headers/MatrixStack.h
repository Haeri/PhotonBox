#ifndef B_MATRIX_STACK_H
#define B_MATRIX_STACK_H

#include <memory>
#include <iostream>
#include "vmmlib/util.hpp"
#include "vmmlib/vector.hpp"


/** @brief This is a matrix stack to temporarily store transformations 
*	@author Benjamin Buergisser
*/
class MatrixStack
{
public:

	/* Functions */

	/**	@brief Constructor
	*/
	MatrixStack() {}

	/**	@brief Virtual destructor
	*/
	virtual ~MatrixStack(){}

	/**	@brief Push a matrix to the stack
	*	@param[in] matrix
	*/
	virtual void pushMatrix(const vmml::Matrix4f &matrix);

	/**	@brief Delete last element on the stack
	*/
	virtual  void popMatrix();

	/**	@brief Deletes all matrices in the stack
	*/
	virtual void clearMatrixStack();

	/**	@brief Returns the matrix as the product of all pushed transformations
	*/
	virtual vmml::Matrix4f getMatrix();

private:

	/* Variables */

    std::vector<vmml::Matrix4f> _matrixStack;
};

typedef std::shared_ptr<MatrixStack> MatrixStackPtr;

#endif /* defined(B_MATRIX_STACK_H) */
