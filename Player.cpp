#include "Player.h"

Player::Player(float x, float y, float angle) {
	m_xPosition = x;
	m_yPosition = y;
	m_angle = angle;
}

const float Player::FOV = 3.141592 / 4.0;
const float Player::ViewDistance = 16.0f;

float Player::GetX() { return m_xPosition; }
float Player::GetY() { return m_yPosition; }
float Player::GetAngle() { return m_angle; }
void Player::IncrementAngle(float deltaAngle) { m_angle += deltaAngle; }