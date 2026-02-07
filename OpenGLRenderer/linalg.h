#pragma once

struct Vec3 {
	float x, y, z;


	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	static float length(Vec3 vec);
	static Vec3 normalize(Vec3 vec);
	static Vec3 scalar(Vec3 vec, float scalar);
	static Vec3 add(Vec3 vec1, Vec3 vec2);
	static Vec3 subtract(Vec3 vec1, Vec3 vec2);
	static Vec3 cross(Vec3 vec1, Vec3 vec2);
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
	float matrix[4][4];

	Matrix4(float diag);
	Matrix4() : Matrix4(0.0f) {}
};

struct LinAlg {
public:
	static float* val_ptr(Matrix4 m);
	static Matrix4 add(Matrix4 m1, Matrix4 m2);
	static Matrix4 scalar(Matrix4 m, float scalar);
	static Matrix4 transpose(Matrix4 m);
	static Matrix4 mult(Matrix4 m1, Matrix4 m2);
	static Matrix4 translate(Matrix4 m, Vec3 translate);
	static Matrix4 rotate(Matrix4 m, float angle, Vec3 rotationAxis);
	static Matrix4 perspective(float fov, float aspect, float near, float far);
	static Matrix4 lookAt(Vec3 camPos, Vec3 target, Vec3 up);
	static void setMatrix2D1D(float* destination, float* matrix, size_t byteCount);
};