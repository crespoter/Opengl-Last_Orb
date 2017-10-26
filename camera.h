#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float ZOOM = 45.0f;
class Camera
{
public:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_cameraUp;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_movementspeed;
	float m_zoom;
	Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW,float pitch = PITCH) : m_front(glm::vec3(0.0f,1.0f,0.0f)),m_movementspeed(SPEED),m_zoom(ZOOM)
	{
		m_position = position;
		m_worldUp = up;
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_worldUp);
	}
	void moveCamera(Camera_Movement direction, float deltaTime)
	{
		float velocity = m_movementspeed * deltaTime;
		if (direction == FORWARD)
			m_position += m_front * velocity;
		else if (direction == BACKWARD)
			m_position -= m_front * velocity;
		else if (direction == LEFT)
			m_position -= m_right * velocity;
		else if (direction == RIGHT)
			m_position += m_right * velocity;
	}
	void rotateCamera(float deltaYaw, float deltaPitch,bool constrainPitch = true)
	{
		m_yaw += deltaYaw;
		m_pitch += deltaPitch;
		if (constrainPitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}
		updateCameraVectors();
	}
	void updateZoom(float deltaZoom)
	{
		if (m_zoom >= 1.0f && m_zoom <= 45.0f)
			m_zoom -= deltaZoom;
		if (m_zoom <= 1.0f)
			m_zoom = 1.0f;
		if (m_zoom >= 45.0f)
			m_zoom = 45.0f;
	}
private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw))* cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_cameraUp = glm::normalize(glm::cross(m_right,m_front));
	}
};

