#include "Planet.hpp"

Planet::Planet(float _mass, float _radius) {
	radius = _radius;
	sphere = Sphere(radius);
	mass = _mass;
}

Planet::Planet(float _mass, float _radius, float _orbitRadius, std::string texturePath, Sun &sun) {
	sphere = Sphere(_radius);
	mass = _mass;
	radius = _radius;
	orbitRadius = _orbitRadius;
	orbitSpeed = sqrt(sun.mass * G / pow(orbitRadius, 3));
    loadTexture(texturePath);
    rotateSpeed = _radius * 10;
}

void Planet::orbit(float deltaTime) {
	position.x = orbitRadius * cos(position.w) / 2e7;
	position.z = orbitRadius * sin(position.w) / 2e7;

	position.w += orbitSpeed * deltaTime * 20;
    angle += rotateSpeed * deltaTime;
}

void Planet::render(Shader & shader, unsigned int depthCubeMap) {
    glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, position.z));
    translate = glm::rotate(translate, angle, glm::vec3(0, 1, 0));

	shader.setMat4("model", translate);
    shader.setInt("texture1", 0);

    if (depthCubeMap >= 0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
        shader.setInt("depthMap", 2);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
	
    sphere.renderSphere();
}

void Planet::loadTexture(std::string texturePath) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}