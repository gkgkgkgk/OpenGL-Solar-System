#include <glew.h>
#include <vector>

class Planet {
public:
	float mass;
	float radius;

	std::vector<GLfloat> getVertices();
};