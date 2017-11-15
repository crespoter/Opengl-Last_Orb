#pragma once
#include<glm\glm.hpp>
#include<iostream>
enum rotationStatus { NOT_ROTATING, ROTATING_LEFT, ROTATING_RIGHT };
enum positionStats {MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN,STOP};
const float minHorizontalPos = -0.65;
const float maxHorizontalPos = 0.65;
const float minVerticalPos = -0.0;
const float maxVerticalPos = 0.5;
const float minRotation = -30.0f;
const float maxRotation = 30.0f;
class PlayerController
{
private:
	rotationStatus r_status;
	positionStats p_status;
	positionStats sm_status;
	float m_speed;
	float m_rotateSpeed;
public:
	glm::vec3 m_position;
	float rotationAngle;
	PlayerController(float x, float y, float z, float speed, float rspeed)
	{
		m_position = glm::vec3(x, y, z);
		r_status = NOT_ROTATING;
		m_speed = speed;
		m_rotateSpeed = rspeed;
		p_status = STOP;
	}
	float startGanking(rotationStatus way,float deltaTime,float minAngle,float maxAngle)
	{
		r_status = way;
		switch (way)
		{
		case NOT_ROTATING:
			if (rotationAngle < - minAngle)
				rotationAngle += m_rotateSpeed*deltaTime;
			else if (rotationAngle > minAngle)
				rotationAngle -= m_rotateSpeed * deltaTime;
			break;
		case ROTATING_LEFT:
			if (rotationAngle > -maxAngle)
				rotationAngle -= m_rotateSpeed*deltaTime;
			break;
		case ROTATING_RIGHT:
			if (rotationAngle < maxAngle)
				rotationAngle += m_rotateSpeed*deltaTime;
			break;
		default:
			std::cout << "WAY UNDEFINED>>ERROR ROTATING PLAYER";
		}
		return rotationAngle;
	}
	float moveLeft(float minPosition, float deltaTime)
	{
		if (m_position.x > minPosition + m_speed*deltaTime)
			m_position.x -= m_speed*deltaTime;
		return m_position.x;
	}
	float moveRight(float maxPosition, float deltaTime)
	{
		if (m_position.x < maxPosition - m_speed*deltaTime)
			m_position.x += m_speed*deltaTime;
		return m_position.x;
	}
	float moveUp(float maxPosition, float deltaTime)
	{
		if (m_position.y < maxPosition - m_speed*deltaTime)
			m_position.y += m_speed*deltaTime;
		return m_position.y;
	}
	float moveDown(float minPosition, float deltaTime)
	{
		if (m_position.y > minPosition - m_speed*deltaTime)
			m_position.y -= m_speed*deltaTime;
		return m_position.y;
	}

	void setStatus(positionStats status)
	{
		p_status = status;
	}
	void setSecondaryStatus(positionStats status)
	{
		sm_status = status;
	}

	glm::mat4 Update(float deltaTime)
	{
		glm::mat4 model;
		switch (p_status)
		{
		case MOVE_LEFT:
			rotationAngle = rotationAngle > 0 ? 0 : rotationAngle;
			switch (sm_status)
			{
			case STOP:
			case MOVE_RIGHT:
				moveLeft(minHorizontalPos, deltaTime);
				startGanking(ROTATING_LEFT, deltaTime, minRotation, maxRotation);
				model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case MOVE_UP:
				moveLeft(minHorizontalPos, deltaTime / 2.0);
				moveUp(maxVerticalPos, deltaTime / 2.0);
				startGanking(ROTATING_LEFT, deltaTime, minRotation, maxRotation);
				model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case MOVE_DOWN:
				moveLeft(minHorizontalPos, deltaTime / 2.0);
				moveDown(minVerticalPos, deltaTime / 2.0);
				startGanking(ROTATING_LEFT, deltaTime, minRotation, maxRotation);
				model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			}
			break;
		case MOVE_RIGHT:
			rotationAngle = rotationAngle < 0 ? 0 : rotationAngle;
			switch(sm_status)
			{
			case STOP:
			case MOVE_LEFT:
				moveRight(maxHorizontalPos, deltaTime);
				startGanking(ROTATING_RIGHT, deltaTime, minRotation, maxRotation);
				model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case MOVE_UP:
				moveUp(maxVerticalPos, deltaTime / 2.0);
				moveRight(maxHorizontalPos, deltaTime/2.0);
				startGanking(ROTATING_RIGHT, deltaTime, minRotation, maxRotation);
				model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case MOVE_DOWN:
				moveDown(minVerticalPos, deltaTime / 2.0);
				moveRight(maxHorizontalPos, deltaTime / 2.0);
				startGanking(ROTATING_RIGHT, deltaTime, minRotation, maxRotation);
				model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			}
			break;
		case MOVE_UP:
			model = glm::translate(model, glm::vec3(m_position.x, moveUp(maxVerticalPos, deltaTime), 0.0));
			break;
		case MOVE_DOWN:
			model = glm::translate(model, glm::vec3(m_position.x, moveDown(minVerticalPos, deltaTime), 0.0));
			break;
		default:
			model = glm::translate(model, m_position );
			rotationAngle = 0;
		}
		return model;
	}

};

