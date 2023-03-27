#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.hpp"
#include "Shader.h"

class Sun {
public:
	float mass = 0;
	Sphere center;
	Sphere glow;
	glm::vec4 position = glm::vec4(0.0f);
	float radius = 1;
	float glowRadius = 2;

	Sun() {};
	Sun(float mass, float radius, float glowRadius);
	void renderGlow(Shader& shader);
	void renderCenter(Shader& shader);
};