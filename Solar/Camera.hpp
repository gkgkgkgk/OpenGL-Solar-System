#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.hpp"
#include "Shader.h"


class Camera {
public:
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = 0;
	float pitch = 0;
	float radius = 5.0;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
	unsigned int bloomTexture;

	Camera() {};
	Camera(float radius);
	void updateCameraPos(float x, float y);
	void initializeSkybox(Shader &skyShader);
	void renderSkybox(Shader &skyShader);
	unsigned int loadCubemap(std::vector<std::string> faces);
};