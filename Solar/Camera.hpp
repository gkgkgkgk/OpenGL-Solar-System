#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

class Camera {
public:
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = 0;
	float pitch = 0;
	float radius = 5.0;
	
	Camera() {};
	Camera(float radius);
	void updateCameraPos(float x, float y);
};