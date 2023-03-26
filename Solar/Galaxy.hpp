#include <vector>
#include "Planet.hpp"
#include "Shader.h"

class Galaxy {
public:
	std::vector<Planet> planets;

	Galaxy() {};
	Galaxy(Planet& sun);
	void addPlanet(Planet& planet);
	void renderGalaxy(Shader &planetShader, Shader& sunShader, float deltaTime);
	void renderGalaxy(Shader& skyShader);
};