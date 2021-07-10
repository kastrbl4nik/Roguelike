#include "Player.h"
#include <math.h>

Player::Player(float x, float y, float angle) {
	m_xPosition = x;
	m_yPosition = y;
	m_angle = angle;
}

Player::~Player()
{
}

const float Player::FOV = 3.141592 / 3.0;
const float Player::ViewDistance = 16.0f;
const float Player::Speed = 8.0f;

void Player::IncrementAngle(float deltaAngle) { m_angle += deltaAngle; }

void Player::Walk(float magnitude, int inputX, int inputY) {
	if (inputX > 0.1f) {
		m_xPosition += magnitude * sinf(m_angle);
		m_yPosition += magnitude * cosf(m_angle);
	}
	else if (inputX < -0.1f) {
		m_xPosition -= magnitude * sinf(m_angle);
		m_yPosition -= magnitude * cosf(m_angle);
	}

	//TODO: Add strafing
}