#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include "Camera.hpp"
#include "Galaxy.hpp"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;
float deltaTime = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float lastX = -1;
float lastY = -1;
Camera camera;
bool firstMouse = true;
Sun sun;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		return -1;
	}

	Shader skyShader("./sky.vertexshader", "./sky.fragmentshader");
	Shader planetShader("./vert.vertexshader", "./frag.fragmentshader");
	Shader sunShader("./sun.vertexshader", "./sun.fragmentshader");
	Shader depthShader("./depth.vertexshader", "./depth.fragmentshader");

	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);

	// shadows
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sun = Sun(2e30, 1, 2);
	camera = Camera(20.0f);
	camera.updateCameraPos(0, 0);
	camera.initializeSkybox(skyShader);
	Galaxy galaxy = Galaxy(sun);

	Planet p = Planet(5.972e24, 0.2, 150e6, "./assets/Martian.png", sun);
	galaxy.addPlanet(p);

	Planet p2 = Planet(5.972e24, 0.15, 48e6, "./assets/Terrestrial4.png", sun);
	galaxy.addPlanet(p2);
	
	Planet p3 = Planet(5.972e24, 0.3, 96e6, "./assets/Gaseous1.png", sun);
	galaxy.addPlanet(p3);

	Planet p4 = Planet(5.972e24, 0.25, 200e6, "./assets/Icy.png", sun);
	galaxy.addPlanet(p4);

	Planet p5 = Planet(5.972e24, 0.1, 65e6, "./assets/Volcanic.png", sun);
	galaxy.addPlanet(p5);

	glm::mat4 model = glm::mat4(1.0f);

	double prevTime = glfwGetTime();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClearColor(0.0f, 0.1f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// SHADOWS
		// -----------------------------------------------
		float near_plane = 0.1f;
		float far_plane = 50.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(glm::vec3(0.0), glm::vec3(0.0) + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(glm::vec3(0.0), glm::vec3(0.0) + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(glm::vec3(0.0), glm::vec3(0.0) + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(glm::vec3(0.0), glm::vec3(0.0) + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(glm::vec3(0.0), glm::vec3(0.0) + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(glm::vec3(0.0), glm::vec3(0.0) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			depthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		depthShader.setFloat("far_plane", far_plane);
		depthShader.setVec3("lightPos", glm::vec3(0.0));
		galaxy.renderGalaxy(depthShader, sunShader, -1, deltaTime);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// -----------------------------------------------


		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		planetShader.use();
		planetShader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);
		planetShader.setVec3("light.diffuse", 3.0f, 3.0f, 3.0f);
		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", camera.view);

		sunShader.use();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", camera.view);
		sunShader.setVec3("cameraPos", camera.position);
		sunShader.setFloat("_time", float(glfwGetTime() * 0.5));

		skyShader.use();
		skyShader.setMat4("projection", projection);
		skyShader.setMat4("view", camera.view);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		camera.renderSkybox(skyShader);
		galaxy.renderGalaxy(planetShader, sunShader, depthCubemap, deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();

		prevTime = currentTime;

	}


	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		camera.updateCameraPos(xoffset, yoffset);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		lastX = -1;
		lastY = -1;
		firstMouse = true;
	}
}