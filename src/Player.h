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
	~Player();

	inline float Player::GetY()     const { return m_yPosition; }
	inline float Player::GetAngle() const { return m_angle; }
	inline float Player::GetX()     const { return m_xPosition; }

	void IncrementAngle(float deltaAngle);
	void Walk(float magnitude, int inputX, int inputY);
};

#endif // !PLAYER_H