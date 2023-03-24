#include <glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Sphere.hpp"
#include "Shader.h"

#define PI 3.1415
#define G 6.6743 * pow(10, -11)

class Planet {
public:
	float mass;
	float radius;
	Sphere sphere;
	glm::vec4 position = glm::vec4(0.0f);

	Planet *sun;

	Planet() {};
	Planet(float mass, float radius);
	Planet(float mass, float radius, float orbitRadius, Planet &sun);
	void orbit(float deltaTime);
	glm::vec3 computeForce();
	void render(Shader &shader);

private:
	float orbitRadius = 2.0f;
	float orbitSpeed = 0;
};