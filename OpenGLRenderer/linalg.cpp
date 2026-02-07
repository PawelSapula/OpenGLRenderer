#include "linalg.h"

#include <cmath>
#include <cstring>

float Vec3::length(Vec3 vec) {
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

Vec3 Vec3::normalize(Vec3 vec) {
	float mag = Vec3::length(vec);
	return Vec3(vec.x / mag, vec.y / mag, vec.z / mag);
}

Vec3 Vec3::scalar(Vec3 vec, float scalar) {
	return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vec3 Vec3::add(Vec3 vec1, Vec3 vec2) {
	return Vec3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

Vec3 Vec3::subtract(Vec3 vec1, Vec3 vec2) {
	return Vec3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

Vec3 Vec3::cross(Vec3 vec1, Vec3 vec2) {
	return Vec3(vec1.y * vec2.z - vec1.z * vec2.y,
		vec1.z * vec2.x - vec1.x * vec2.z,
		vec1.x * vec2.y - vec1.y * vec2.x);
}

Matrix4::Matrix4(float diag) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			float val = row == column ? diag : 0.0f;
			this->matrix[row][column] = val;
		}
	}
}

Matrix4 LinAlg::add(Matrix4 m1, Matrix4 m2) {

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			m1.matrix[row][column] += m2.matrix[row][column];
		}
	}

	return m1;
}

Matrix4 LinAlg::scalar(Matrix4 m, float scalar) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			m.matrix[row][column] *= scalar;
		}
	}

	return m;
}

Matrix4 LinAlg::transpose(Matrix4 m) {
	Matrix4 transponded;
	size_t size = sizeof(Matrix4) / 16; // For later mby if its necessary to make different types than 4x4 matrices. (sizeof(float)*sizeof(float) didnt work rn)
	for (int row = 0; row < size; row++) {
		for (int column = 0; column < size; column++) {
			
			transponded.matrix[column][row] = m.matrix[row][column];

		}
	}
	return transponded;
}

Matrix4 LinAlg::mult(Matrix4 m1, Matrix4 m2) {
	size_t size = sizeof(Matrix4) / 16;

	Matrix4 result;

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {
			result.matrix[row][col] = 0.0f;

			for (int k = 0; k < 4; k++) {
				result.matrix[row][col] += m1.matrix[row][k] * m2.matrix[k][col];
			}

		}
	}

	return result;
}

Matrix4 LinAlg::translate(Matrix4 m, Vec3 translate) {

	m.matrix[3][0] = translate.x;
	m.matrix[3][1] = translate.y;
	m.matrix[3][2] = translate.z;

	return m;
}

void LinAlg::setMatrix2D1D(float* destination, float* matrix, std::size_t byteCount) {
	std::memcpy(destination, matrix, byteCount);
}

float* LinAlg::val_ptr(Matrix4& m) {
	m = LinAlg::transpose(m); // Turn to column major
	return &m.matrix[0][0];
}

Matrix4 LinAlg::rotate(Matrix4 m, float angle, Vec3 rotationAxis) {

	if (rotationAxis.x != 0.0f) {
		float Rx[16] = { 1,	0,	0, 0,
			0, cos(angle), -sin(angle), 0,
			0, sin(angle), cos(angle), 0,
			0,	0,	0,	1 };

		Matrix4 tempR;
		setMatrix2D1D(&tempR.matrix[0][0], Rx, sizeof(Matrix4));
		m = mult(m, tempR);

	}

	if (rotationAxis.y != 0.0f) {
		float Ry[16] = { cos(angle),	0,	sin(angle), 0,
			0, 1, 0, 0,
			-sin(angle), 0, cos(angle), 0,
			0,	0,	0,	1 };

		Matrix4 tempR;
		setMatrix2D1D(&tempR.matrix[0][0], Ry, sizeof(Matrix4));
		m = mult(m, tempR);

	}

	if (rotationAxis.z != 0.0f) {
		float Rz[16] = {cos(angle),	-sin(angle), 0, 0,
			sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0,	0,	0,	1 };

		Matrix4 tempR;
		setMatrix2D1D(&tempR.matrix[0][0], Rz, sizeof(Matrix4));
		m = mult(m, tempR);

	}

	return m;

}

Matrix4 LinAlg::perspective(float fov, float aspect, float near, float far) { // OpenGL uses column-major order!!! This is a transposed matrix (Moved to val_ptr, now its row-major).
	float perspective[] = { 1.0f / (aspect * tanf(fov / 2)),		0,					0,									0,
							0,									1 / tanf(fov / 2),		0,									0,
							0,										0,			-(far + near) / (far - near),				-(2 * far * near) / (far - near),
							0,										0,					-1,									0 };

	Matrix4 m;
	setMatrix2D1D(&m.matrix[0][0], perspective, sizeof(Matrix4));
	return m;
}

Matrix4 LinAlg::lookAt(Vec3 camPos, Vec3 target, Vec3 up) { // Fuck this shit ive wasted probably like 10 hours now on this shit fuck opengl and its column major crap ass shit and fuck learnopengl and the owner and fuck opengl and mathematics and everything around me.
	Vec3 camDir = Vec3::normalize(Vec3::subtract(camPos, target)); //points target -> camera, opposite
	Vec3 camRight = Vec3::normalize(Vec3::cross(up, camDir)); // right
	Vec3 camUp = Vec3::cross(camDir, camRight); // up


	float rot[] = {	// World axes to camera axes
		camRight.x, camRight.y, camRight.z, 0,
		camUp.x,	camUp.y,	camUp.z,	0,
		camDir.x,	camDir.y,	camDir.z,	0,
			0,			0,			0,		1
	};


	Matrix4 rotMat;
	setMatrix2D1D(&rotMat.matrix[0][0], rot, sizeof(Matrix4));

	float translate[] = {	// Opposite of camera position
		1, 0, 0, -camPos.x,
		0, 1, 0, -camPos.y,
		0, 0, 1, -camPos.z,
		0, 0, 0, 1
	};

	Matrix4 translateMatrix;
	setMatrix2D1D(&translateMatrix.matrix[0][0], translate, sizeof(Matrix4));

	rotMat = LinAlg::mult(rotMat, translateMatrix); // rotate and then transate

	return rotMat;

}

