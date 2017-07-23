#include "Camera.h"

//Helper functions
glm::mat4 Camera::calculateOrientationMatrix() const {
	glm::mat4 matrix = glm::transpose(glm::mat4(
		glm::vec4(this->right, 0.0f),
		glm::vec4(this->up, 0.0f),
		glm::vec4(this->target, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	));
	return matrix;
}

glm::mat4 Camera::calculateViewMatrix() const {
	return this->orientationMatrix * glm::translate(glm::mat4(), this->position);
}

//Constructors and deconstructors
Camera::Camera() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->target = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(1.0, 0.0, 0.0);
	this->right = glm::vec3(0.0, 1.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->fieldOfView = 90.0f;
}

Camera::Camera(float fieldOfView) {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->target = glm::vec3(0.0, 0.0, 1.0);
	this->up = glm::vec3(1.0, 0.0, 0.0);
	this->right = glm::vec3(0.0, 1.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->fieldOfView = fieldOfView;
}

Camera::~Camera() {

}

//Getters and setters
glm::vec3 Camera::GetPosition() const {
	return this->position;
}

glm::vec3 Camera::GetTarget() const {
	return this->target;
}

void Camera::SetPosition(const glm::vec3 &position) {
	this->position = position;
}

void Camera::SetTarget(const glm::vec3 &target) {
	this->target = target;
}

//Functions
void Camera::Update(float moveSpeed, float rotationSpeed, float deltaTime, InputManager* inputManager) {
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	//Update movement based on keys pressed
	if (inputManager->KeyPressed('A')) {
		movement -= this->right;
	}
	if (inputManager->KeyPressed('D')) {
		movement += this->right;
	}
	if (inputManager->KeyPressed('W')) {
		movement += this->target;
	}
	if (inputManager->KeyPressed('S')) {
		movement -= this->target;
	}

	//Update rotation based on mouse movement
	if (inputManager->MouseMoved()) {
		glm::vec2 deltaMove = inputManager->MouseDeltaMovement();
		rotation.x = deltaMove.x; // TODO: Update these values
		rotation.y = deltaMove.y;
	}
	
	this->position += movement * moveSpeed * deltaTime;
	
	Yaw(rotation.x);
	Pitch(rotation.y);

	this->orientationMatrix = calculateOrientationMatrix();
	this->viewMatrix = calculateViewMatrix();
}

void Camera::Yaw(float rotation) {
	glm::quat quaternion = glm::angleAxis(glm::radians(rotation), up);
	this->target = glm::normalize(quaternion * target);
	this->right = glm::normalize(quaternion * right);
}

void Camera::Pitch(float rotation) {
	glm::quat quaternion = glm::angleAxis(glm::radians(rotation), right);
	this->target = glm::normalize(quaternion * target);
	this->up = glm::normalize(quaternion * up);
}