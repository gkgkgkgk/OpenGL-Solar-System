#include "Galaxy.hpp"

Galaxy::Galaxy(Planet& sun) {
	addPlanet(sun);
}

void Galaxy::addPlanet(Planet& planet) {
	this->planets.push_back(planet);
}

void Galaxy::renderGalaxy(Shader& planetShader, float deltaTime) {
	for (int i = 1; i < planets.size(); i++) {
		planets[i].orbit(deltaTime);
	}
	
	for (int i = 0; i < planets.size(); i++) {
		planets[i].render(planetShader);
	}
}