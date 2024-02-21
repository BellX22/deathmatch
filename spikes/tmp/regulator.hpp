#pragma once

class Regulator
{
public:
	Regulator(float timeout = 0.f) : m_timeout(timeout), m_elapsed(0.f) {}

	void update(float dt) {
		m_elapsed += dt;
	}

	bool isReady() {
		if (m_elapsed > m_timeout) {
			m_elapsed -= m_timeout;
			return true;
		}
		else {
			return false;
		}
	}

private:
	float m_timeout;
	float m_elapsed;
};
