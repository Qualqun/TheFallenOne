#include "Text.hpp"

Text::Text()
{
	m_font.loadFromFile("Font/Chapbook-Regular.otf");
}

Text::~Text()
{

}


void Text::Init(TextInfo _text)
{
	if (_text.m_string != "No Name")
	{
		TextInfo tempText;
		tempText.m_string = _text.m_string;
		tempText.m_text.setOrigin(0, 0);
		tempText.m_text.setCharacterSize(_text.m_size);
		tempText.m_screenPos = _text.m_pos;
		tempText.m_text.setPosition(_text.m_pos);
		tempText.m_text.setFillColor(_text.m_color);
		tempText.m_text.setString(_text.m_string);
		tempText.m_totalTime = (tempText.m_string.size() / GLYPH_PER_SECOND) + SECOND_LASTING_TEXT;
		tempText.m_currentTime = 0.f;

		m_textData.push_back(tempText);
	}

}

void Text::Update(CamData& _camData, float _dt)
{
	sf::Vector2f camOffset = _camData.GetPosition() - _camData.GetCamera().getSize() / 2.f;

	for (size_t i = 0; i < m_textData.size(); i++)
	{
		m_textData[i].m_currentTime += _dt;
		sf::String sousTexte = m_textData[i].m_string.substr(0, m_textData[i].m_currentTime * GLYPH_PER_SECOND);

		m_textData[i].m_pos.x = camOffset.x + m_textData[i].m_screenPos.x;
		m_textData[i].m_pos.y = camOffset.y + m_textData[i].m_screenPos.y;
		m_textData[i].m_text.setPosition(m_textData[i].m_pos);
		m_textData[i].m_text.setString(sousTexte);
	}
}

void Text::Display(sf::RenderWindow& _window)
{
	for (int i = 0; i < m_textData.size(); i++)
	{
		if (m_textData[i].m_currentTime < m_textData[i].m_totalTime)
		{
			m_textData[i].m_text.setFont(m_font);
			_window.draw(m_textData[i].m_text);
		}
		else if (m_textData[i].m_currentTime >= m_textData[i].m_totalTime)
		{
			m_textData.erase(m_textData.begin() + i);
		}
	}
}