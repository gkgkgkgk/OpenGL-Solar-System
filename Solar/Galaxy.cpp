#include "Galaxy.hpp"

Galaxy::Galaxy() {

}

Galaxy::Galaxy(Sun& _sun) {
	sun = _sun;
}

void Galaxy::addPlanet(Planet& planet) {
	this->planets.push_back(planet);
}

void Galaxy::renderGalaxy(Shader& planetShader, Shader& sunShader, unsigned int depthCubeMap, float deltaTime) {
	if (depthCubeMap == -1) {
		planetShader.use();

		for (int i = 0; i < planets.size(); i++) {
			planets[i].render(planetShader, depthCubeMap);
		}
	}
	else {
		sunShader.use();
		sun.renderCenter(sunShader);
		for (int i = 0; i < planets.size(); i++) {
			planets[i].orbit(deltaTime);
		}
		glEnable(GL_DEPTH_TEST);

		planetShader.use();


		for (int i = 0; i < planets.size(); i++) {
			planets[i].render(planetShader, depthCubeMap);
		}

		sunShader.use();
		sun.renderGlow(sunShader);
	}
}