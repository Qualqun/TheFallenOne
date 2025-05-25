#include "Common.h"

b2Body* CreateBodyB2D(b2World* _world, B2Dinfo _b2dInfo)
{
	b2BodyDef bodyDef;
	bodyDef.type = _b2dInfo.type;
	bodyDef.position.Set(_b2dInfo.pos.x, _b2dInfo.pos.y);
	bodyDef.fixedRotation = _b2dInfo.fixedRotation;

	if (_b2dInfo.shape == CIRCLE)
	{

		b2Body* body = _world->CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_radius = _b2dInfo.radius;

		b2FixtureDef fixturedef;


		fixturedef.userData.pointer = (uintptr_t) new UserData(_b2dInfo.userData);
		fixturedef.shape = &shape;
		fixturedef.density = _b2dInfo.density;
		fixturedef.friction = _b2dInfo.friction;

		body->CreateFixture(&fixturedef);

		return body;
	}

	if (_b2dInfo.shape == POLYGON)
	{
		b2Body* body = _world->CreateBody(&bodyDef);

		int32 nbPoints = _b2dInfo.points.size();
		b2PolygonShape shape;

		b2Vec2* points = new b2Vec2[nbPoints];

		for (int i = 0; i < nbPoints; i++)
		{
			points[i] = _b2dInfo.points[i];
		}

		shape.Set(points, nbPoints);

		b2FixtureDef fixturedef;


		fixturedef.userData.pointer = (uintptr_t) new UserData(_b2dInfo.userData);
		fixturedef.shape = &shape;
		fixturedef.density = _b2dInfo.density;
		fixturedef.friction = _b2dInfo.friction;


		body->CreateFixture(&fixturedef);

		return body;
	}
}

b2Body* CreateBodyB2D(b2World* _world, B2Dinfo _b2dInfo, uint16 _categoryBits, uint16 _maskBits = 0xFFFF)
{
	if (_b2dInfo.shape == CIRCLE)
	{
		b2BodyDef bodyDef;
		bodyDef.type = _b2dInfo.type;
		bodyDef.position.Set(_b2dInfo.pos.x, _b2dInfo.pos.y);
		bodyDef.fixedRotation = _b2dInfo.fixedRotation;

		b2Body* body = _world->CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_radius = _b2dInfo.radius;

		b2FixtureDef fixturedef;


		fixturedef.userData.pointer = (uintptr_t) new UserData(_b2dInfo.userData);

		fixturedef.shape = &shape;
		fixturedef.density = _b2dInfo.density;
		fixturedef.friction = _b2dInfo.friction;

		fixturedef.filter.maskBits = _maskBits;
		fixturedef.filter.categoryBits = _categoryBits;
		body->CreateFixture(&fixturedef);

		return body;
	}

	if (_b2dInfo.shape == POLYGON)
	{

		b2BodyDef bodyDef;
		bodyDef.type = _b2dInfo.type;
		bodyDef.position.Set(_b2dInfo.pos.x, _b2dInfo.pos.y);
		bodyDef.fixedRotation = _b2dInfo.fixedRotation;

		b2Body* body = _world->CreateBody(&bodyDef);

		int32 nbPoints = _b2dInfo.points.size();
		b2PolygonShape shape;
		b2Vec2* points = new b2Vec2[nbPoints];

		for (int i = 0; i < nbPoints; i++)
		{
			points[i] = _b2dInfo.points[i];
		}

		shape.Set(points, nbPoints);

		b2FixtureDef fixturedef;


		fixturedef.userData.pointer = (uintptr_t) new UserData(_b2dInfo.userData);

		fixturedef.shape = &shape;
		fixturedef.density = _b2dInfo.density;
		fixturedef.friction = _b2dInfo.friction;

		fixturedef.filter.maskBits = _maskBits;
		fixturedef.filter.categoryBits = _categoryBits;

		body->CreateFixture(&fixturedef);

		return body;
	}
}
b2Body* CreateProjectile(b2World* _world, float _radius, float _density, float _speed, sf::Vector2f _pos)
{

	b2BodyDef DynamicBodyDef;
	DynamicBodyDef.type = b2_dynamicBody;
	DynamicBodyDef.position.Set(_pos.x / 64, -_pos.y / 64);
	b2Body* projectile = _world->CreateBody(&DynamicBodyDef);




	b2CircleShape circleCollider;
	circleCollider.m_radius = _radius;

	int count = 4;



	b2FixtureDef fixtCircle;
	fixtCircle.density = 3;
	fixtCircle.friction = 0.3;
	fixtCircle.shape = &circleCollider;
	projectile->CreateFixture(&fixtCircle);
	return projectile;
}
b2Body* CreateProjectile(b2World* _world, float _radius, float _density, float _speed, sf::Vector2f _pos, std::string _name)
{

	b2BodyDef DynamicBodyDef;
	DynamicBodyDef.type = b2_dynamicBody;
	DynamicBodyDef.position.Set(_pos.x / 64, -_pos.y / 64);
	b2Body* projectile = _world->CreateBody(&DynamicBodyDef);




	b2CircleShape circleCollider;
	circleCollider.m_radius = _radius;

	int count = 4;



	b2FixtureDef fixtCircle;
	fixtCircle.density = 3;
	fixtCircle.friction = 0.3;
	fixtCircle.shape = &circleCollider;
	fixtCircle.userData.pointer = (uintptr_t) new std::string(_name);
	projectile->CreateFixture(&fixtCircle);
	return projectile;
}
sf::Sprite LoadSprite(const std::string _path, bool _isCentered)
{
	//Création et chargement de la texture
	sf::Texture* tempTex = new sf::Texture(); //Nécessaire dynamiquement
	tempTex->loadFromFile(_path);

	//Création et configuration du sprite
	sf::Sprite tempSprite;
	tempSprite.setTexture(*tempTex);

	if (_isCentered) //Centrage du sprite
	{
		sf::Vector2u tempTaille = tempTex->getSize();
		sf::Vector2f tempOrigin = sf::Vector2f(tempTaille.x / 2, tempTaille.y / 2);
		tempSprite.setOrigin(tempOrigin);
	}

	return tempSprite;
}

sf::SoundBuffer LoadSoundBuffer(const std::string _path)
{
	sf::SoundBuffer soundTemp;

	soundTemp.loadFromFile(_path);

	return soundTemp;
}

void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window)
{
	_sprite.setRotation(_angle);
	_sprite.setPosition(_pos);
	_window.draw(_sprite);
}

void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window, sf::Shader& _shader)
{
	_sprite.setRotation(_angle);
	_sprite.setPosition(_pos);
	_window.draw(_sprite, &_shader);
}


void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window, int _transparency = 255)
{
	_sprite.setRotation(_angle);
	_sprite.setPosition(_pos);
	_sprite.setColor(sf::Color(255, 255, 255, _transparency));
	_window.draw(_sprite);
}

void BlitText(sf::Text _text, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window)
{
	_text.setRotation(_angle);
	_text.setPosition(_pos);
	_window.draw(_text);
}

sf::Vector2f World2Screen(b2Vec2 _worldPos, sf::Vector2f _cam = { 0 , 0 })
{
	sf::Vector2f screenPos = sf::Vector2f(_worldPos.x - _cam.x, _worldPos.y - _cam.y);

	screenPos.y = -screenPos.y; //Inversion de l'axe Y pour coller à box2D
	screenPos.x *= ZOOM;
	screenPos.y *= ZOOM;

	return screenPos;
}
sf::Vector2f World2Screen(b2Vec2 _worldPos)
{
	sf::Vector2f screenPos = sf::Vector2f(_worldPos.x, _worldPos.y);

	screenPos.y = -screenPos.y; //Inversion de l'axe Y pour coller à box2D
	screenPos.x *= ZOOM;
	screenPos.y *= ZOOM;

	return screenPos;
}
sf::Vector2f World2ScreenVec(sf::Vector2f _screenPos)
{
	sf::Vector2f screenPos = sf::Vector2f(_screenPos.x, _screenPos.y);

	screenPos.y = -screenPos.y; //Inversion de l'axe Y pour coller à box2D
	screenPos.x *= ZOOM;
	screenPos.y *= ZOOM;

	return screenPos;
}
b2Vec2 Screen2World(sf::Vector2f _screenPos, sf::Vector2f _cam = { 0 , 0 })
{
	b2Vec2 worldPos = b2Vec2(_screenPos.x - _cam.x, _screenPos.y - _cam.y);

	worldPos.y = -worldPos.y; //Inversion de l'axe Y pour coller à box2D
	worldPos.x /= ZOOM;
	worldPos.y /= ZOOM;

	return worldPos;
}
b2Vec2 Screen2World(sf::Vector2f _screenPos)
{
	b2Vec2 worldPos = b2Vec2(_screenPos.x, _screenPos.y);

	worldPos.y = -worldPos.y; //Inversion de l'axe Y pour coller à box2D
	worldPos.x /= ZOOM;
	worldPos.y /= ZOOM;

	return worldPos;
}
sf::Vector2f Screen2WorldVec(sf::Vector2f _screenPos)
{
	sf::Vector2f worldPos = sf::Vector2f(_screenPos.x, _screenPos.y);

	worldPos.y = -worldPos.y; //Inversion de l'axe Y pour coller à box2D
	worldPos.x /= ZOOM;
	worldPos.y /= ZOOM;

	return worldPos;
}

float Pixel2Meter(float _pixels)
{
	return _pixels /= ZOOM;
}

float Meter2Pixel(float _meters)
{
	return _meters *= ZOOM;
}

bool CollideCirclePoint(b2Vec2 _circlePos, float _radius, b2Vec2 _point)
{
	float distance = sqrt(pow(_point.x - _circlePos.x, 2) + pow(_point.y - _circlePos.y, 2));

	return distance <= _radius / 2;
}

b2Body* GetBodyFromWorld(std::string _nameFixture, b2World* _world)
{
	b2Body* allBody = _world->GetBodyList();

	for (int i = 0; i < _world->GetBodyCount(); i++)
	{
		if (allBody != nullptr && &allBody->GetFixtureList()[0] != nullptr)
		{
			if (*((std::string*)allBody->GetFixtureList()[0].GetUserData().pointer) == _nameFixture)
			{
				return allBody;
			}
			else
			{
				allBody = allBody->GetNext();
			}
		}
	}

	return nullptr;
}

b2Body* GetBodyOfMap(b2World* _world, std::string _elementName)
{
	b2Body* allBody = _world->GetBodyList();

	for (int i = 0; i < _world->GetBodyCount(); i++)
	{
		if (allBody != nullptr && &allBody->GetFixtureList()[0] != nullptr)
		{
			UserData userDataBody = *((UserData*)allBody->GetFixtureList()[0].GetUserData().pointer);
			if (userDataBody.partOfMap == _elementName)
			{
				return allBody;
			}
			else
			{
				allBody = allBody->GetNext();
			}
		}
	}

	return nullptr;
}
bool isBodyOnScreen(b2Body* _body)
{
	sf::Vector2f bodyPos = { _body->GetPosition().x * ZOOM, _body->GetPosition().y * ZOOM };
	if (bodyPos.x < 0 || bodyPos.x > SCREEN_WIDTH || bodyPos.y < 0 || bodyPos.y > SCREEN_HEIGHT)
	{
		return false;
	}
	else
	{
		return true;
	}


}
float DegToRad(double _angle)
{
	return _angle * M_PI / 180.f;
}

float RadToDeg(double _angle)
{
	return _angle * 180 / M_PI;
}

sf::Vector2f VecDirToVecUnit(sf::Vector2f _vectorDir)
{
	sf::Vector2f unitVec = { 0,0 };
	float norm = sqrtf(_vectorDir.x * _vectorDir.x + _vectorDir.y * _vectorDir.y);
	unitVec = _vectorDir / norm;

	return unitVec;
}