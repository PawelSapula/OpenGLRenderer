#include "linalg.h"

#include <cmath>


Matrix4::Matrix4(float diag) {
	int i = 0;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			float val = row == column ? diag : 0.0f;
			this->matrix[i] = val;
			i++;
		}
	}
}

Matrix4 LinAlg::add(Matrix4 m1, Matrix4 m2) {
	for (int i = 0; i < sizeof(Matrix4)/sizeof(float); i++) {
		m1.matrix[i] += m2.matrix[i];
	}

	return m1;
}

Matrix4 LinAlg::scalar(Matrix4 m, float scalar) {
	for (int i = 0; i < sizeof(Matrix4) / sizeof(float); i++) {
		m.matrix[i] *= scalar;
	}

	return m;
}

Matrix4 LinAlg::transpose(Matrix4 m) {
	Matrix4 transponded;
	size_t size = sizeof(Matrix4) / 16; // For later mby if its necessary to make different types than 4x4 matrices. (sizeof(float)*sizeof(float) didnt work rn)
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			
			transponded.matrix[row * 4 + col] = m.matrix[col * 4 + row]; // Thanks chat didnt work when i was debugging my indexing

		}
	}
	return transponded;
}

Matrix4 LinAlg::mult(Matrix4 m1, Matrix4 m2) {
	size_t size = sizeof(Matrix4) / 16;
	m2 = transpose(m2); // For easier mental image, at the end of the day didn't work a single inch

	Matrix4 result;

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			result.matrix[row * 4 + col] = m1.matrix[row * 4] * m2.matrix[col + 4*0] +  // Could solve this after an hour or so, had to get help from chat
				m1.matrix[row * 4 + 1] * m2.matrix[col + 4 * 1] +
				m1.matrix[row * 4 + 2] * m2.matrix[col + 4 * 2] +
				m1.matrix[row * 4 + 3] * m2.matrix[col + 4 * 3];

		}
	}

	return result;
}

Matrix4 LinAlg::translate(Matrix4 m, Vec3 translate) {
	m.matrix[12] = translate.x;
	m.matrix[13] = translate.y;
	m.matrix[14] = translate.z;

	return m;
}

void LinAlg::setMatrix(float* destination, float* matrix, size_t count) {
	for (int i = 0; i < count; i++) {
		destination[i] = matrix[i];
	}
}

Matrix4 LinAlg::rotate(Matrix4 m, float angle, Vec3 rotationAxis) {

	if (rotationAxis.x != 0.0f) {
		float Rx[16] = { 1,	0,	0, 0,
			0, cos(angle), -sin(angle), 0,
			0, sin(angle), cos(angle), 0,
			0,	0,	0,	1 };

		Matrix4 tempR;
		setMatrix(tempR.matrix, Rx, sizeof(Matrix4)/sizeof(float));
		m = mult(m, tempR);

	}

	if (rotationAxis.y != 0.0f) {
		float Ry[16] = { cos(angle),	0,	sin(angle), 0,
			0, 1, 0, 0,
			-sin(angle), 0, cos(angle), 0,
			0,	0,	0,	1 };

		Matrix4 tempR;
		setMatrix(tempR.matrix, Ry, sizeof(Matrix4) / sizeof(float));
		m = mult(m, tempR);

	}

	if (rotationAxis.z != 0.0f) {
		float Rz[16] = {cos(angle),	-sin(angle), 0, 0,
			-sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0,	0,	0,	1 };

		Matrix4 tempR;
		setMatrix(tempR.matrix, Rz, sizeof(Matrix4) / sizeof(float));
		m = mult(m, tempR);

	}

	return m;

}

Matrix4 LinAlg::perspective(float fov, float aspect, float near, float far) { // OpenGL uses column-major order!!!
	float perspective[] = { 1.0f / (aspect * tanf(fov / 2)),		0,					0,									0,
							0,									1 / tanf(fov / 2),		0,									0,
							0,										0,			-(far + near) / (far - near),				-1,
							0,										0,			-(2 * far * near) / (far - near),			0 };

	Matrix4 m;
	setMatrix(m.matrix, perspective, sizeof(Matrix4) / sizeof(float));
	return m;
}

