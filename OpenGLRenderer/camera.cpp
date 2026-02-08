#include "camera.h"

#include "gl.h";

Camera::Camera(Vec3 Position, Vec3 WorldUp, float yaw, float pitch) {
	this->Position = Position;
	this->WorldUp = WorldUp;
	this->Yaw = yaw;
	this->Pitch = pitch;

	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVITY;
	this->FOV = CFOV;

	this->updateCameraVectors();
}

void Camera::updateCameraVectors() {

	Vec3 direction;
	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	this->Front = Vec3::normalize(direction);

	this->Right = Vec3::normalize(Vec3::cross(this->Front, this->WorldUp));
	this->Up = Vec3::normalize(Vec3::cross(this->Right, this->Front));
}

Matrix4 Camera::GetViewMatrix() {

	return LinAlg::lookAt(
		this->Position,
		Vec3::add(this->Position, this->Front), // The camera keeps looking in forwards. The target is in front of us and moves with the position.
		this->Up
	);

}

void Camera::ProcessKeyboard(Movement direction, float deltaTime) {

	const float speed = this->MovementSpeed * deltaTime;

	if (direction == FORWARD) {
		this->Position = Vec3::add(this->Position, Vec3::scalar(this->Front, speed)); // Go forward in z axis -> add negative z, camera object - z
	}
	if (direction == BACKWARD) {
		this->Position = Vec3::subtract(this->Position, Vec3::scalar(this->Front, speed)); // Go back in z axis -> camera object + z
	}
	if (direction == LEFT) {

		this->Position = Vec3::subtract(this->Position, Vec3::scalar(Right, speed)); // Go positive x

	}
	if (direction == RIGHT) {
		this->Position = Vec3::add(this->Position, Vec3::scalar(Right,speed)); // Go positive x
	}
	if (direction == UP) {
		this->Position = Vec3::add(this->Position, Vec3::scalar(this->Up, speed));
	}
	if (direction == DOWN) {
		this->Position = Vec3::subtract(this->Position, Vec3::scalar(this->Up, speed));
	}

}
void Camera::processScroll(float y) {
	FOV -= (float)y;
	if (FOV < 1.0f) FOV = 1.0f;
	if (FOV > 60.0f) FOV = 60.0f;
}


void Camera::ProcessMouse(float xOffset, float yOffset) {
	xOffset *= this->MouseSensitivity;
	yOffset *= this->MouseSensitivity;

	Yaw += xOffset; // Add sensitivity offset. Can directly add because proportional to screen up-down, left-right because of differentiation in the callback.
	Pitch += yOffset;

	//Limit the viewing angles because of a weird flip in LookAt when VcamDir = VworldUp.
	if (Pitch > 89.0f) Pitch = 89.0f;
	if (Pitch < -89.0f) Pitch = -89.0f;

	this->updateCameraVectors();
}