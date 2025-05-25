#include "Animation.hpp"
#define SIZE_SPAWN_ANIM_X_MAX 9100
#define SIZE_SPAWN_ANIM_Y_MAX 5050

void Animation(sf::Sprite& _sprite, Anim* _anim, const unsigned int _state, const float _dt, const int _sizeMaxImage)
{
	_anim->timeAnim += _dt;

	float frameTime = 1.f / _anim[_state].framerate;
	int frameNb = _anim->timeAnim / frameTime;
	int heightSum = 0;


	if (_anim->timeAnim > frameTime * frameNb)
	{
		int posX;
		if (frameNb > _anim[_state].frameNb - 1)
		{
			frameNb = _anim[_state].frameNb - 1;
		}
		for (int i = 0; i < _state; i++)
		{
			heightSum += _anim[i].texRect.y;
		}
		for (int i = 0; i <= _state; i++)
		{
			heightSum += _anim[i].offset;
		}
		posX = _anim[_state].texRect.x * frameNb;

		if (_sizeMaxImage <= posX)
		{
			int nbLine = posX / _sizeMaxImage;

			posX -= _sizeMaxImage * nbLine;
			heightSum += _anim[_state].texRect.y * nbLine;
		}

		sf::IntRect textureRect = { posX, heightSum, _anim[_state].texRect.x, _anim[_state].texRect.y };
		_sprite.setTextureRect(textureRect);

		if (_anim[_state].isLooping == true && _anim->timeAnim > frameTime * _anim[_state].frameNb && frameNb == _anim[_state].frameNb - 1)
		{
			_anim->timeAnim = 0;
		}
	}
}

int GetActualyFrame(Anim* _anim, int _state)
{
	float frameTime = 1.f / _anim[_state].framerate;
	int frameNb = _anim->timeAnim / frameTime;

	return frameNb;
}

sf::IntRect GetAnimIntRect(Anim* _anim, const unsigned int _state)
{
	float frameTime = 1.f / _anim[_state].framerate;
	int frameNb = _anim[_state].timeAnim / frameTime;
	int heightSum = 0;
	sf::IntRect textureRect;

	if (_anim[_state].timeAnim > frameTime * frameNb)
	{
		int posX;
		if (frameNb > _anim[_state].frameNb - 1)
		{
			frameNb = _anim[_state].frameNb - 1;
		}
		for (int i = 0; i < _state; i++)
		{
			heightSum += _anim[i].texRect.y;
		}
		for (int i = 0; i <= _state; i++)
		{
			heightSum += _anim[i].offset;
		}

		posX = _anim[_state].texRect.x * frameNb;

		textureRect = { posX, heightSum, _anim[_state].texRect.x, _anim[_state].texRect.y };

		return textureRect;
	}
	else
	{
		return sf::IntRect{ {0, 0}, _anim->texRect };
	}
}
void AnimationChain(sf::Sprite& _sprite, Anim* _anim, const unsigned int _state, const float _dt)
{
	_anim->timeAnim += _dt;

	float frameTime = 1.f / _anim[_state].framerate;
	int frameNb = _anim->timeAnim / frameTime;
	int heightSum = 0;


	if (_anim->timeAnim > frameTime * frameNb)
	{
		int posX;
		if (frameNb > _anim[_state].frameNb - 1)
		{
			frameNb = _anim[_state].frameNb - 1;
		}
		for (int i = 0; i < _state; i++)
		{
			heightSum += _anim[i].texRect.y;
		}
		for (int i = 0; i <= _state; i++)
		{
			heightSum += _anim[i].offset;
		}

		posX = _anim[_state].texRect.x * frameNb;

		sf::IntRect textureRect = { posX, heightSum, _anim[_state].texRect.x, _anim[_state].texRect.y };

		_sprite.setTextureRect(textureRect);

		if (_anim[_state].isLooping == true && _anim->timeAnim > frameTime * _anim[_state].frameNb && frameNb == _anim[_state].frameNb - 1)
		{
			_anim->timeAnim = 0;
		}

		if (_anim[_state].isLooping == false && _anim->timeAnim > frameTime * _anim[_state].frameNb && frameNb == _anim[_state].frameNb - 1)
		{
			_anim->timeAnim = frameTime * _anim[_state].frameNb;
		}
	}
}
void AnimationSkull(sf::Sprite& _sprite, Anim* _anim, const unsigned int _state, const float _dt)
{
	_anim[_state].timeAnim += _dt;

	float frameTime = 1.f / _anim[_state].framerate;
	int frameNb = _anim[_state].timeAnim / frameTime;
	int heightSum = 0;


	if (_anim[_state].timeAnim > frameTime * frameNb)
	{
		int posX;
		if (frameNb > _anim[_state].frameNb - 1)
		{
			frameNb = _anim[_state].frameNb - 1;
		}
		for (int i = 0; i < _state; i++)
		{
			heightSum += _anim[i].texRect.y;
		}
		for (int i = 0; i <= _state; i++)
		{
			heightSum += _anim[i].offset;
		}

		posX = _anim[_state].texRect.x * frameNb;

		sf::IntRect textureRect = { posX, heightSum, _anim[_state].texRect.x, _anim[_state].texRect.y };

		_sprite.setTextureRect(textureRect);

		if (_anim[_state].isLooping == true && _anim[_state].timeAnim > frameTime * _anim[_state].frameNb && frameNb == _anim[_state].frameNb - 1)
		{
			_anim[_state].timeAnim = 0;
		}

		if (_anim[_state].isLooping == false && _anim[_state].timeAnim > frameTime * _anim[_state].frameNb && frameNb == _anim[_state].frameNb - 1)
		{
			_anim[_state].timeAnim = frameTime * _anim[_state].frameNb;
		}
	}
}
void AnimationSpawnBoss(sf::Sprite& _sprite, Anim* _anim, unsigned int _state, const float _dt)
{
	_anim->timeAnim += _dt;

	float frameTime = 1.f / _anim[_state].framerate;
	int frameNb = _anim->timeAnim / frameTime;
	int heightSum = 0;


	if (_anim->timeAnim > frameTime * frameNb)
	{
		int posX;
		if (frameNb > _anim[_state].frameNb - 1)
		{
			frameNb = _anim[_state].frameNb - 1;
		}
		for (int i = 0; i < _state; i++)
		{
			heightSum += _anim[i].texRect.y;
		}
		for (int i = 0; i <= _state; i++)
		{
			heightSum += _anim[i].offset;
		}
		posX = _anim[_state].texRect.x * frameNb;
		if (SIZE_SPAWN_ANIM_X_MAX - _anim[_state].texRect.x < posX)
		{
			posX -= SIZE_SPAWN_ANIM_X_MAX - 910;
			heightSum += _anim[_state].texRect.y;
		}
		sf::IntRect textureRect = { posX, heightSum, _anim[_state].texRect.x, _anim[_state].texRect.y };

		_sprite.setTextureRect(textureRect);

		if (_anim[_state].isLooping == true && _anim->timeAnim > frameTime * _anim[_state].frameNb && frameNb == _anim[_state].frameNb - 1)
		{
			_anim->timeAnim = 0;
		}
	}
}
Anim InitAnimation(sf::Vector2i _textureRect, const unsigned int _framerate, const unsigned int _frameNb, const unsigned int _offsetY, const bool _isLooping)
{
	Anim anim;
	anim.texRect = _textureRect;
	anim.framerate = _framerate;
	anim.frameNb = _frameNb;
	anim.offset = _offsetY;
	anim.isLooping = _isLooping;
	anim.timeAnim = 0;
	return anim;
}