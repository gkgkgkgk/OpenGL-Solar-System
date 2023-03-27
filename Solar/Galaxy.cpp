#include "Galaxy.hpp"

Galaxy::Galaxy() {

}

Galaxy::Galaxy(Sun& _sun) {
	sun = _sun;
}

void Galaxy::addPlanet(Planet& planet) {
	this->planets.push_back(planet);
}

void Galaxy::renderGalaxy(Shader& planetShader, Shader& sunShader, float deltaTime) {
	sunShader.use();
	sun.renderCenter(sunShader);
	for (int i = 0; i < planets.size(); i++) {
		planets[i].orbit(deltaTime);
	}
	glEnable(GL_DEPTH_TEST);

	planetShader.use();
	// bind textures on corresponding texture units

	for (int i = 0; i < planets.size(); i++) {
		planets[i].render(planetShader);
	}

	sunShader.use();
	sun.renderGlow(sunShader);
}