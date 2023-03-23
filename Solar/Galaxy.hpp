#include <vector>
#include "Planet.hpp"

class Galaxy {
public:
	std::vector<Planet> planets;
	
	Galaxy();
	void addPlanet(Planet& planet);
	void simulateGalaxy();
	void renderGalaxy();
};