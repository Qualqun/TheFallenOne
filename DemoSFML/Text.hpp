#ifndef TEXT_HPP
#define TEXT_HPP

#include "Common.h"
#include "Camera.hpp"


class Text
{
public:
	Text();
	~Text();

	void Init(TextInfo _text);
	void Update(CamData& _camData, float _dt);
	void Display(sf::RenderWindow& _window);

private:
	std::vector<TextInfo> m_textData;
	sf::Font m_font;
};

#endif