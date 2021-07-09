#pragma once
#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	float m_xPosition;
	float m_yPosition;
	float m_angle;
public:
	static const float FOV;
	static const float ViewDistance;
	static const float Speed;

	Player(float x, float y, float angle);
	float GetX();
	float GetY();
	float GetAngle();
	void IncrementAngle(float deltaAngle);

	void Walk(float magnitude, int inputX, int inputY);
};

#endif // !PLAYER_H