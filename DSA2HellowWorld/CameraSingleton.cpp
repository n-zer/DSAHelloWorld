#include "CameraSingleton.h"

void CameraSingleton::SetScreenSize(int width, int height) {
	screenWidth = width;
	screenHeight = height;
}
glm::mat4 CameraSingleton::GetView(void) {
	return glm::lookAt(position, position + target, up);
}

glm::mat4 CameraSingleton::GetProjection(bool bOrtographic) {
	if (bOrtographic)
		return glm::ortho(0, screenWidth, 0, screenHeight);
	else
		return glm::perspective(glm::radians(70.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.f);
}

void CameraSingleton::SetPosition(glm::vec3 v3Position) {
	position = v3Position;
}

void CameraSingleton::SetTarget(glm::vec3 v3Target) {
	target = v3Target;
	right = glm::cross(target, up); //recalculate right
}

void CameraSingleton::SetUp(glm::vec3 v3Up) {
	up = v3Up;
	right = glm::cross(target, up);//recalculate right
}

void CameraSingleton::MoveForward(float fIncrement) {
	position += glm::normalize(target)*fIncrement;
}

void CameraSingleton::MoveSideways(float fIncrement) {
	position += glm::normalize(right)*fIncrement;
}

void CameraSingleton::MoveVertical(float fIncrement) {
	position += glm::normalize(up)*fIncrement;
}

void CameraSingleton::ChangePitch(float fIncrement) {
	RotateAlongAxis(glm::normalize(right), -fIncrement);
}

void CameraSingleton::ChangeRoll(float fIncrement) {
	RotateAlongAxis(glm::normalize(target), -fIncrement);
}

void CameraSingleton::ChangeYaw(float fIncrement) {	
	RotateAlongAxis(glm::normalize(up), fIncrement);
}

void CameraSingleton::RotateAlongAxis(glm::vec3 axis, float increment) {
	float angle = -glm::radians(increment); //get angle in radians
	//compute quat components
	float w = cosf(angle / 2.0f);
	float x = axis.x*sinf(angle / 2.0f); //axis is the axis of rotation
	float y = axis.y*sinf(angle / 2.0f);
	float z = axis.z*sinf(angle / 2.0f);
	glm::quat cameraRotation = glm::quat(w, x, y, z);

	//apply quat to target and up, recalculate right
	target = cameraRotation * target;
	up = cameraRotation * up;
	right = glm::cross(target, up);
}