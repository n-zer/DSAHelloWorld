#ifndef _CAMERASINGLETON
#define _CAMERASINGLETON
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class CameraSingleton
{
public:
	static CameraSingleton& getInstance()
	{
		static CameraSingleton instance; // Guaranteed to be destroyed.
										 // Instantiated on first use.
		return instance;
	}
private:
	CameraSingleton() {
		position = glm::vec3(0, 0, 0);
		target = glm::vec3(1, 0, 0);
		up = glm::vec3(0, 1, 0);
		right = glm::cross(target, up);
		screenWidth = 1024;
		screenHeight = 768;
	};
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 right;
	int screenWidth;
	int screenHeight;
	void RotateAlongAxis(glm::vec3 axis, float increment);
public:
	CameraSingleton(CameraSingleton const&) = delete;
	void operator=(CameraSingleton const&) = delete;

	glm::mat4 GetView(void);
	glm::mat4 GetProjection(bool bOrtographic);
	void SetScreenSize(int width, int height);
	void SetPosition(glm::vec3 v3Position);
	void SetTarget(glm::vec3 v3Target);
	void SetUp(glm::vec3 v3Up);
	void MoveForward(float fIncrement);
	void MoveSideways(float fIncrement);
	void MoveVertical(float fIncrement);
	void ChangePitch(float fIncrement);
	void ChangeRoll(float fIncrement);
	void ChangeYaw(float fIncrement);
};

#endif