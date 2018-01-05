#include "Camera.h"

void Camera::init(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
{
	carAcc = 0.0f;
	carYaw = 0.0f;
	carPosition = vec3(0.0f, -0.5f, 0.0f);

	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	carFront = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;
	updateCameraVectors();
}

void Camera::init(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;

	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Idle(float deltaTime)
{
	vec3 move = vec3(0);

	move = -vec3(0.1f * cos(glm::radians(carYaw)), 0, 0.1f * sin(glm::radians(carYaw))) * carAcc;

	carPosition += move;

	if (carAcc < 0) {
		carAcc += deltaTime * 0.7f;
		if (carAcc > 0) carAcc = 0;
		if (carAcc < -2.5f) carAcc = -2.5f;
	}
	if (carAcc > 0) {
		carAcc -= deltaTime * 0.7f;
		if (carAcc < 0) carAcc = 0;
		if (carAcc > 6) carAcc = 6;
	}

	updateCameraVectors();
}

void Camera::setControl()
{
	control = control ? 0 : 1;
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	float posZ = Position.y;
	GLfloat velocity = MovementSpeed * deltaTime;

	float rotateSpeed = 20;

	//carYaw = rotateSpeed * velocity;
	float yaw = 0.0f;

	if (carAcc > 0.2f) {
		if (direction == LEFT) {
			yaw -= rotateSpeed * velocity;
		}
		if (direction == RIGHT) {
			yaw += rotateSpeed * velocity;
		}
	}
	else if (carAcc < -0.2f) {
		if (direction == LEFT) {
			yaw += rotateSpeed * velocity;
		}
		if (direction == RIGHT) {
			yaw -= rotateSpeed * velocity;
		}
	}

	carYaw += yaw;

	float factor = 3;
	if (direction == FORWARD) {
		if (carAcc == 0) {
			carAcc = 0.6f;
		}

		if (carAcc < 0) {
			factor = 3;
		}

		carAcc += deltaTime * factor;
	}
	if (direction == BACKWARD) {
		if (carAcc == 0) {
			carAcc = -0.6f;
		}

		if (carAcc > 0) {
			factor = 3;
		}

		carAcc -= deltaTime * factor;
	}

	//if (direction == FORWARD) {
	//	//Position += Front * velocity;
	//}
	//if (direction == BACKWARD) {
	//	move = vec3(0.1f * sin(glm::radians(carYaw)), 0, 0.1f * cos(glm::radians(carYaw))) * carAcc;
	//	//Position -= Front * velocity;
	//}

	updateCameraVectors();
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset * SENSITIVTY_SCROLL;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	if (!control) {
		Yaw = 180 + carYaw;

		Position.x = carPosition.x + cos(glm::radians(carYaw)) * 8;
		Position.y = carPosition.y + 5;
		Position.z = carPosition.z + sin(glm::radians(carYaw)) * 8;
	}

	// Calculate the new Front vector
	glm::vec3 front;

	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));

	// Calculate the new Front vector
	carFront = Front;

	carFront.x = cos(glm::radians(180 + carYaw)) * cos(glm::radians(0.7f));
	carFront.y = sin(glm::radians(-60.0f));
	carFront.z = sin(glm::radians(180 + carYaw)) * cos(glm::radians(0.7f));
	carFront = glm::normalize(carFront);
}