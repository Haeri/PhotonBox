#ifndef MATH_TEST_H
#define MATH_TEST_H

#include "math/Matrix4f.h"

class MathTest
{
public:
	static void startTest()
	{
		std::cout << "==================================================" << std::endl;
		std::cout << "                   STARTING TESTS" << std::endl << std::endl;

		// ---------------------- PERSPECTIVE TEST  ---------------------- //
		std::cout << "Perspective Test\t\t - ";
		Matrix4f perspective = Matrix4f::createPerspective(45.0f, 1.33333337, 0.1f, 50.0);
		Matrix4f perspectvieResult;
		perspectvieResult(0, 0) = 1.81066012f;	perspectvieResult(1, 0) = 0;			perspectvieResult(2, 0) = 0;			perspectvieResult(3, 0) = 0;
		perspectvieResult(0, 1) = 0;			perspectvieResult(1, 1) = 2.41421342f;	perspectvieResult(2, 1) = 0;			perspectvieResult(3, 1) = 0;
		perspectvieResult(0, 2) = 0;			perspectvieResult(1, 2) = 0;			perspectvieResult(2, 2) = -1.00400794f;	perspectvieResult(3, 2) = -0.200400800f;
		perspectvieResult(0, 3) = 0;			perspectvieResult(1, 3) = 0;			perspectvieResult(2, 3) = -1;			perspectvieResult(3, 3) = 0;

		if (perspective == perspectvieResult)
		{
			std::cout << "PASS" << std::endl;
		}
		else
		{
			std::cout << "FAILD!" << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "Expected: " << std::endl << perspectvieResult << std::endl << std::endl << "Result: " << std::endl << perspective << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}



		// ---------------------- LOOKAT TEST  ---------------------- //
		std::cout << "Look At Test\t\t\t - ";
		Matrix4f lookAt = Matrix4f::lookAt(Vector3f(3, 3, -3), Vector3f(0, 1, 0), Vector3f(-3, -3, 3).normalize());
		Matrix4f lookAtResult;
		lookAtResult(0, 0) = -0.707106769f;		lookAtResult(1, 0) = 0.0f;				lookAtResult(2, 0) = -0.707106769f;		lookAtResult(3, 0) = 0.0f;
		lookAtResult(0, 1) = -0.408248276f;		lookAtResult(1, 1) = 0.816496551f;		lookAtResult(2, 1) = 0.408248276f;		lookAtResult(3, 1) = 0.0f;
		lookAtResult(0, 2) = 0.577350259f;		lookAtResult(1, 2) = 0.577350259f;		lookAtResult(2, 2) = -0.577350259f;		lookAtResult(3, 2) = -5.19615221f;
		lookAtResult(0, 3) = 0.0f;				lookAtResult(1, 3) = 0.0f;				lookAtResult(2, 3) = 0.0f;				lookAtResult(3, 3) = 1.0f;

		if (lookAt == lookAtResult)
		{
			std::cout << "PASS" << std::endl;
		}
		else
		{
			std::cout << "FAILD!" << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "Expected: " << std::endl << lookAtResult << std::endl << std::endl << "Result: " << std::endl << lookAt << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}


		// ---------------------- TRANSPOSE TEST  ---------------------- //
		std::cout << "Transpose Test\t\t\t - ";
		Matrix4f originalTranspose;
		originalTranspose(0, 0) = 1;		originalTranspose(1, 0) = 5;		originalTranspose(2, 0) = 9;		originalTranspose(3, 0) = 13;
		originalTranspose(0, 1) = 2;		originalTranspose(1, 1) = 6;		originalTranspose(2, 1) = 10;		originalTranspose(3, 1) = 14;
		originalTranspose(0, 2) = 3;		originalTranspose(1, 2) = 7;		originalTranspose(2, 2) = 11;		originalTranspose(3, 2) = 15;
		originalTranspose(0, 3) = 4;		originalTranspose(1, 3) = 8;		originalTranspose(2, 3) = 12;		originalTranspose(3, 3) = 16;
		originalTranspose.transpose();
		Matrix4f transposeResult;
		transposeResult(0, 0) = 1;		transposeResult(1, 0) = 2;		transposeResult(2, 0) = 3;		transposeResult(3, 0) = 4;
		transposeResult(0, 1) = 5;		transposeResult(1, 1) = 6;		transposeResult(2, 1) = 7;		transposeResult(3, 1) = 8;
		transposeResult(0, 2) = 9;		transposeResult(1, 2) = 10;		transposeResult(2, 2) = 11;		transposeResult(3, 2) = 12;
		transposeResult(0, 3) = 13;		transposeResult(1, 3) = 14;		transposeResult(2, 3) = 15;		transposeResult(3, 3) = 16;

		if (originalTranspose == transposeResult)
		{
			std::cout << "PASS" << std::endl;
		}
		else
		{
			std::cout << "FAILD!" << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "Expected: " << std::endl << transposeResult << std::endl << std::endl << "Result: " << std::endl << originalTranspose << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}




		// ---------------------- MATRIX MULTIPLICATION TEST  ---------------------- //
		std::cout << "Matrix Multiplication Test\t - ";
		Matrix4f originalMMulti1;
		originalMMulti1(0, 0) = 1;		originalMMulti1(1, 0) = 5;		originalMMulti1(2, 0) = 9;		originalMMulti1(3, 0) = 13;
		originalMMulti1(0, 1) = 2;		originalMMulti1(1, 1) = 6;		originalMMulti1(2, 1) = 10;		originalMMulti1(3, 1) = 14;
		originalMMulti1(0, 2) = 3;		originalMMulti1(1, 2) = 7;		originalMMulti1(2, 2) = 11;		originalMMulti1(3, 2) = 15;
		originalMMulti1(0, 3) = 4;		originalMMulti1(1, 3) = 8;		originalMMulti1(2, 3) = 12;		originalMMulti1(3, 3) = 16;
		Matrix4f originalMMulti2;
		originalMMulti2(0, 0) = 1;		originalMMulti2(1, 0) = 5;		originalMMulti2(2, 0) = 9;		originalMMulti2(3, 0) = 13;
		originalMMulti2(0, 1) = 2;		originalMMulti2(1, 1) = 6;		originalMMulti2(2, 1) = 10;		originalMMulti2(3, 1) = 14;
		originalMMulti2(0, 2) = 3;		originalMMulti2(1, 2) = 7;		originalMMulti2(2, 2) = 11;		originalMMulti2(3, 2) = 15;
		originalMMulti2(0, 3) = 4;		originalMMulti2(1, 3) = 8;		originalMMulti2(2, 3) = 12;		originalMMulti2(3, 3) = 16;
		Matrix4f multiRes = originalMMulti1 * originalMMulti2;
		Matrix4f mmultiSolution;
		mmultiSolution(0, 0) = 90;		mmultiSolution(1, 0) = 202;		mmultiSolution(2, 0) = 314;		mmultiSolution(3, 0) = 426;
		mmultiSolution(0, 1) = 100;		mmultiSolution(1, 1) = 228;		mmultiSolution(2, 1) = 356;		mmultiSolution(3, 1) = 484;
		mmultiSolution(0, 2) = 110;		mmultiSolution(1, 2) = 254;		mmultiSolution(2, 2) = 398;		mmultiSolution(3, 2) = 542;
		mmultiSolution(0, 3) = 120;		mmultiSolution(1, 3) = 280;		mmultiSolution(2, 3) = 440;		mmultiSolution(3, 3) = 600;

		if (multiRes == mmultiSolution)
		{
			std::cout << "PASS" << std::endl;
		}
		else
		{
			std::cout << "FAILD!" << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "Expected: " << std::endl << mmultiSolution << std::endl << std::endl << "Result: " << std::endl << multiRes << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}



		// ---------------------- VECTOR MULTIPLICATION TEST  ---------------------- //

		std::cout << "Vector Multiplication Test\t - ";
		Matrix4f originalVMulti;
		originalVMulti(0, 0) = 1;		originalVMulti(1, 0) = 5;		originalVMulti(2, 0) = 9;		originalVMulti(3, 0) = 13;
		originalVMulti(0, 1) = 2;		originalVMulti(1, 1) = 6;		originalVMulti(2, 1) = 10;		originalVMulti(3, 1) = 14;
		originalVMulti(0, 2) = 3;		originalVMulti(1, 2) = 7;		originalVMulti(2, 2) = 11;		originalVMulti(3, 2) = 15;
		originalVMulti(0, 3) = 4;		originalVMulti(1, 3) = 8;		originalVMulti(2, 3) = 12;		originalVMulti(3, 3) = 16;
		Vector4f originalVector(1, 2, 3, 4);
		Vector4f vecRes = originalVMulti * originalVector;
		Vector4f vmultiSolution(90, 100, 110, 120);

		if (vecRes == vmultiSolution)
		{
			std::cout << "PASS" << std::endl;
		}
		else
		{
			std::cout << "FAILD!" << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "Expected: " << std::endl << vmultiSolution << std::endl << std::endl << "Result: " << std::endl << multiRes << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}

		std::cout << std::endl << "                   TESTS ENDED" << std::endl;
		std::cout << "==================================================" << std::endl << std::endl;
	}
private:
};
#endif // MATH_TEST_H
