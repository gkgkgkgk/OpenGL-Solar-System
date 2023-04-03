#include <glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include <iostream>
#include "Sun.hpp"
#include "Sphere.hpp"
#include "Shader.h"
#include "stb_image.hpp"

#define PI 3.1415
#define G 6.6743 * pow(10, -11)

class Planet {
public:
	float mass = 0;
	float radius = 0;
	Sphere sphere;
	glm::vec4 position = glm::vec4(0.0f);
	float rotateSpeed = 0;
	float angle = 0;

	unsigned int texture;

	Planet *sun;

	Planet() {};
	Planet(float mass, float radius);
	Planet(float mass, float radius, float orbitRadius, std::string texturePath, Sun &sun);
	void orbit(float deltaTime);
	void render(Shader &shader, unsigned int depthCubeMap);
	void loadTexture(std::string texturePath);

private:
	float orbitRadius = 2.0f;
	float orbitSpeed = 0;
};