#include <vector>
#include <glew.h>
#include <glm/glm.hpp>

class Sphere {
public:
	float radius;
	std::vector<glm::vec3> vertices;
	std::vector<int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<float> data;

	unsigned int VAO, EBO, VBO;

	int indexCount = 0;

	Sphere() {};
	Sphere(float radius);
	void renderSphere();
};