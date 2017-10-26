#define STB_IMAGE_IMPLEMENTATION
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float backgroundYTranslate = 0;
float backgroundMovementSpeed = 0.1f;

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
	backgroundMesh.addVertices(backgroundVertices, backgroundIndices,sizeof(backgroundVertices),sizeof(backgroundIndices));
	mesh backgroundBufferMesh;
	backgroundBufferMesh.addVertices(backgroundBufferVertices, backgroundIndices, sizeof(backgroundBufferVertices), sizeof(backgroundIndices));
	unsigned int backgroundTextureObject = loadTexture("./Data/Textures/backgroundMain.jpg");
	backgroundShader.use();
	backgroundShader.setInt("texture0", 0);

	model playerModel("./Data/Models/ship_player/Sample_Ship.obj.blend","ship_player");

	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		backgroundYTranslate -= backgroundMovementSpeed * deltaTime;
		if (backgroundYTranslate <= -2)
		{
			backgroundYTranslate = 0;
		}
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		//processInput(window);
		
		//BACKGROUND SETUP
		backgroundShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		backgroundShader.setMat4("projection", projection);
		backgroundShader.setMat4("view", view);
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(1,1,1));
		model = glm::translate(model, glm::vec3(0, backgroundYTranslate, 0));
		backgroundShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backgroundTextureObject);
		backgroundShader.use();
		backgroundMesh.Draw();
		backgroundBufferMesh.Draw();
		
		//PLAYER SETUP
		playerShader.use();
		playerShader.setMat4("projection", projection);
		playerShader.setMat4("view", view);
		glm::mat4 pmodel;
		pmodel = glm::translate(pmodel, glm::vec3(0.0f, -0.25f, 0.0f));
		pmodel = glm::scale(pmodel, glm::vec3(0.25, 0.25, 0.25));
		pmodel = glm::rotate(pmodel, glm::radians(-90.0f), glm::vec3(0.0f,0.0f,1.0f));
		pmodel = glm::rotate(pmodel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		playerShader.setMat4("model", pmodel);
		playerModel.Draw(playerShader);


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
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}