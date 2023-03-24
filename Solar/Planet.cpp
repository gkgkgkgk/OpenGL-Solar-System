#include "Planet.hpp"

Planet::Planet(float _mass, float _radius) {
	radius = _radius;
	sphere = Sphere(radius);
	mass = _mass;
}

Planet::Planet(float _mass, float _radius, float _orbitRadius, Planet &sun) {
	sphere = Sphere(_radius);
	mass = _mass;
	radius = _radius;
	orbitRadius = _orbitRadius;
	orbitSpeed = sqrt(sun.mass * G / pow(orbitRadius, 3));
}

void Planet::orbit(float deltaTime) {
	position.x = orbitRadius * cos(position.w) / 2e7;
	position.z = orbitRadius * sin(position.w) / 2e7;

	position.w += orbitSpeed * deltaTime * 10;
}

void Planet::render(Shader & shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));
	shader.setMat4("model",  model);
	sphere.renderSphere();
}