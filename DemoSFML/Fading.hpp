#ifndef FADING_HPP
#define FADING_HPP

#include "Common.h"
#include "Camera.hpp"

enum FadeType
{
	FADE,
	UNFADE
};

class FadingData
{
private:
	sf::RectangleShape m_fade;
	float m_timeTotal;
	float m_timer;

	bool m_isFinished;

	FadeType m_type;
public:
	FadingData();
	~FadingData();

	void Update(float _dt, CamData& _camData);
	void Draw(sf::RenderWindow& _window);
	void StartFading(float _timeToFade);
	void StartDefading(float _timeToUnfade);

	//Return false if the fading is in progress, return true if the screen has faded
	bool GetStatus() { return m_isFinished; }
	void SetStatus(bool _isFinished) { m_isFinished = _isFinished; }
};



#endif