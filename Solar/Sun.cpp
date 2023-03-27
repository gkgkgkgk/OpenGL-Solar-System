#include "Sun.hpp"

Sun::Sun(float _mass, float _radius, float _glowRadius) {
	radius = _radius;
	glowRadius = _glowRadius;
	mass = _mass;

	glow = Sphere(glowRadius);
	center = Sphere(radius);
}

void Sun::renderGlow(Shader &shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));
	shader.setMat4("model", model);
	shader.use();

	shader.setInt("isGlow", 1);
	glow.renderSphere();
}

void Sun::renderCenter(Shader& shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));
	shader.setMat4("model", model);
	shader.use();


	shader.setInt("isGlow", 0);
	center.renderSphere();
}