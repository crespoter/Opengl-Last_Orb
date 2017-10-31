#define STB_IMAGE_IMPLEMENTATION
#define DEBUG true
#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include<iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "playerController.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float backgroundYTranslate = 0;
float backgroundMovementSpeed = 0.1f;
PlayerController player(0,0,0,0.4,80);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Last Orb", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "ERROR CREATING WINDOW >>>> ABORTING ";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	









	Shader meteorShader("meteorShader.vs", "meteorShader.fs");
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	meteorShader.use();








	Shader backgroundShader("backgroundShader.vs", "backgroundShader.fs");
	Shader playerShader("playerShader.vs", "playerShader.fs");
	Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));


	float backgroundBufferVertices[] =
	{
		-1.0,3.0,0.0,      0.0,1.0,
		1.0,1.0,0.0,       1.0,0.0,
		-1.0,1.0,0.0,      0.0,0.0,
		1.0,3.0,0.0,       1.0,1.0
	};
	float backgroundVertices[] =
	{
		-1.0,1.0,0.0,		0.0,1.0,
		1.0,-1.0,0.0,       1.0,0.0,
		-1.0,-1.0,0.0,      0.0,0.0,
		1.0,1.0,0.0,		1.0,1.0
	};
	unsigned int backgroundIndices[] =
	{
		0,1,2,
		0,3,1
	};
	mesh backgroundMesh;
	backgroundMesh.addVertices(backgroundVertices, backgroundIndices, sizeof(backgroundVertices), sizeof(backgroundIndices));
	mesh backgroundBufferMesh;
	backgroundBufferMesh.addVertices(backgroundBufferVertices, backgroundIndices, sizeof(backgroundBufferVertices), sizeof(backgroundIndices));
	unsigned int backgroundTextureObject = loadTexture("./Data/Textures/backgroundMain.jpg");
	backgroundShader.use();
	backgroundShader.setInt("texture0", 0);

	model playerModel("./Data/Models/ship_player/ship_1.obj", "ship_player");


	//LIGHTING FOR PLAYER

	playerShader.use();
	playerShader.setVec3("dirLight.direction", 0.0f, 0.5f, 0.0f);
	playerShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
	playerShader.setVec3("dirLight.diffuse",0.5f,0.5f, 0.5f);
	playerShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	playerShader.setFloat("material.shininess", 64.0f);


	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		//DEBUG
		playerShader.use();
		glm::vec3 meteorPosition = glm::vec3(5 * glm::sin((float)glfwGetTime()), 5 * glm::cos((float)glfwGetTime()),0.6f);

		playerShader.setVec3("viewPos", camera.m_position);
		playerShader.setVec3("pointlight.position", meteorPosition);
		playerShader.setVec3("pointlight.ambient", 0.01*1.0f, 0.01* 0.27f, 0.0);
		playerShader.setVec3("pointlight.diffuse", 1.5f * 1.0f, 1.5f * 0.27f, 0.0f);
		playerShader.setVec3("pointlight.specular", 3.0f*1.0f, 3.0f*0.27f, 3.0f*0.0f);
		playerShader.setFloat("pointlight.constant", 0.1f);
		playerShader.setFloat("pointlight.linear", 0.09);
		playerShader.setFloat("pointlight.quadratic", 0.032);
		//DEBUG END
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		backgroundYTranslate -= backgroundMovementSpeed * deltaTime;
		if (backgroundYTranslate <= -2)
		{
			backgroundYTranslate = 0;
		}
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);

		//BACKGROUND SETUP
		backgroundShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		backgroundShader.setMat4("projection", projection);
		backgroundShader.setMat4("view", view);
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
		model = glm::translate(model, glm::vec3(0, backgroundYTranslate, -0.1));
		backgroundShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backgroundTextureObject);
		backgroundShader.use();
		backgroundMesh.Draw();
		backgroundBufferMesh.Draw();

		//PLAYER SETUP
		glEnable(GL_CULL_FACE);
		playerShader.use();
		playerShader.setMat4("projection", projection);
		playerShader.setMat4("view", view);

		glm::mat4 pmodel = player.Update(deltaTime);
		pmodel = glm::translate(pmodel, glm::vec3(0.0f, -0.25f, 0.0f));
		pmodel = glm::rotate(pmodel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		pmodel = glm::rotate(pmodel, glm::radians(-90.0f), glm::vec3(0.0f, 0.0, 1.0f));	
		pmodel = glm::scale(pmodel, glm::vec3(0.2, 0.2, 0.2));
		playerShader.setMat4("model", pmodel);
		playerModel.Draw(playerShader);
		glDisable(GL_CULL_FACE);

		//METEOR SETUP
		meteorShader.use();
		glBindVertexArray(lightVAO);
		meteorShader.setMat4("projection", projection);
		meteorShader.setMat4("view", view);
		glm::mat4 meteorModel;
		meteorModel = glm::scale(meteorModel, glm::vec3(0.06, 0.06, 0.06));
		meteorModel = glm::translate(meteorModel, meteorPosition);
		meteorShader.setMat4("model", meteorModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	backgroundMesh.Delete();
	glfwTerminate();
	return 0;
}


unsigned int loadTexture(const char* path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, NULL);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		player.setStatus(MOVE_LEFT);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		player.setStatus(MOVE_RIGHT);
	else
		player.setStatus(STOP);

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}