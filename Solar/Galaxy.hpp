#include <vector>
#include "Planet.hpp"
#include "Sun.hpp"
#include "Shader.h"

class Galaxy {
public:
	std::vector<Planet> planets;
	Sun sun;

	Galaxy();
	Galaxy(Sun& sun);
	void addPlanet(Planet& planet);
	void renderGalaxy(Shader &planetShader, Shader& sunShader, float deltaTime);
};