#pragma once

struct Vec3 {
	float x, y, z;

	Vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct Vec4 {
	float x, y, z, w;

	Vec4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

struct Matrix4 {
	float matrix[16];

	Matrix4(float diag);
	Matrix4() : Matrix4(0.0f) {}
};

struct LinAlg {
public:
	static Matrix4 add(Matrix4 m1, Matrix4 m2);
	static Matrix4 scalar(Matrix4 m, float scalar);
	static Matrix4 transpose(Matrix4 m);
	static Matrix4 mult(Matrix4 m1, Matrix4 m2);
	static Matrix4 translate(Matrix4 m, Vec3 translate);
	static Matrix4 rotate(Matrix4 m, float angle, Vec3 rotationAxis);
	static Matrix4 perspective(float fov, float aspect, float near, float far);
	static void setMatrix(float* destination, float* matrix, size_t count);
};