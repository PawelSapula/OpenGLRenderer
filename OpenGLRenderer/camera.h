#pragma once

#include "linalg.h"

enum Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW			= -90.0f; // <-- Pointing straight in the negative z axis
const float PITCH		= 0.0;
const float SPEED		= 2.5f;
const float SENSITIVITY = 0.1f;
const float CFOV		= 45.0f;

class Camera {
private:
	void updateCameraVectors();

public:
	//Camera attributes

	/*
	For your knowledge you should know that these coordinates are in world-space not view space for moving the camera.
	*/

	Vec3 Position;
	Vec3 Front; 
	Vec3 Right; 
	Vec3 Up;
	Vec3 WorldUp;

	//Angles
	float Yaw;
	float Pitch;
	
	//Camera Options
	float  MovementSpeed;
	float MouseSensitivity;
	float FOV;

	Camera(Vec3 Position = Vec3(0.0f, 0.0f, 0.0f),
		Vec3 Up = Vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f);

	Matrix4 GetViewMatrix();
	void ProcessKeyboard(Movement direction, float deltaTime);
	void processScroll(float y);
	void ProcessMouse(float xOffset, float yOffse);

};