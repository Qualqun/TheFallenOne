#include "Camera.hpp"

void InitCamera(CamData& _camera)
{

}

void CamData::Update(float _dt, sf::Vector2f _posTargetFollow)
{
	//Move the camera to the point needed when you interpolat it
	if (!m_isCameraArrived)
	{
		MoveCameraSmooth(_dt);
	}
	if (m_shouldCameraFollowObject)
	{
		//Follow the player on the point
		//_camera.SetPosition(_posTargetFollow + _camera.GetOffset());

		//Follow the player smoothly with a 0.5 sec travel time
		InterpolCameraTo(_posTargetFollow, 0.5);
		MoveCameraSmooth(_dt);

		m_shouldCameraFollowObject = true;
	}

	//Check if the camera go outsize of the playable zone
	sf::Vector2f camSize = m_camera.getSize();
	sf::Vector2f camPos = m_camera.getCenter();

	std::pair<sf::Vector2f, sf::Vector2f> cameraRange = m_cameraRange;

	if (camPos.x - camSize.x / 2 < m_cameraRange.first.x)
	{
		SetPosition({ m_cameraRange.first.x + camSize.x / 2, camPos.y });
	}
	if (camPos.x + camSize.x / 2 > m_cameraRange.first.y)
	{
		SetPosition({ m_cameraRange.first.y - camSize.x / 2, camPos.y });
	}
	//Update the cam pos
	camPos = m_camera.getCenter();
	if (camPos.y - camSize.y / 2 < m_cameraRange.second.x)
	{
		SetPosition({ camPos.x,  m_cameraRange.second.x + camSize.y / 2 });
	}
	if (camPos.y + camSize.y / 2 > m_cameraRange.second.y)
	{
		SetPosition({ camPos.x,  m_cameraRange.second.y - camSize.y / 2 });
	}

	if (m_shakeTimeLeft > 0)
	{
		m_shakeTimeLeft -= _dt;
		m_shakeTimer -= _dt;

		//m_shakeSpeed = m_baseShakeSpeed * (m_shakeTimeLeft / m_timeToShake);

		if (m_shakeTimer <= 0)
		{
			m_shakeTimer = 1 / m_shakeSpeed;

			sf::Vector2f randShake = { (float)(rand() % (m_intensity * 2) - m_intensity), (float)(rand() % (m_intensity * 2) - m_intensity) };

			//std::cout << "x: " << randShake.x << "\ty: " << randShake.y << std::endl;

			MoveCamera(randShake);
		}
	}


	//Freecam triger
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2))
	{
		m_shouldCameraFollowObject = false;
		m_cameraRange = { { (float)INT_MIN,(float)INT_MAX} ,{ (float)INT_MIN,(float)INT_MAX} };
	}
	//Manual movement
	float multiplier = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
	{
		multiplier = 2 * m_zoom;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_camera.move({ 0,-CAM_SPEED * _dt * multiplier });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_camera.move({ 0,CAM_SPEED * _dt * multiplier });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_camera.move({ -CAM_SPEED * _dt * multiplier,0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_camera.move({ CAM_SPEED * _dt * multiplier,0 });
	}
}

void CamData::MoveCamera(sf::Vector2f _pos)
{
	m_camera.move(_pos + m_cameraOffset);
}

void CamData::MouseWheelScrolledCamera(sf::RenderWindow& _window, sf::Event& _event)
{
	if (_event.type == sf::Event::MouseWheelScrolled)
	{
		if (_event.mouseWheelScroll.delta < 0)
		{
			m_camera.zoom(1.05);
		}
		else if (_event.mouseWheelScroll.delta > 0)
		{
			m_camera.zoom(0.95);
		}
	}
}

void CamData::MoveCameraSmooth(float _dt)
{
	m_timer += _dt;
	if (m_timer >= m_timeToMove)
	{
		m_timer = m_timeToMove;
	}

	sf::Vector2f vectorDistance = { m_cameraFinalPoint.x - m_cameraStartingPoint.x , m_cameraFinalPoint.y - m_cameraStartingPoint.y };

	if (m_shouldCameraGoBackToOrigin)
	{
		SetPosition(vectorDistance * sinf(m_timer * M_PI * 0.5 / (m_timeToMove / 2)) + m_cameraStartingPoint);
	}
	else
	{
		SetPosition(vectorDistance * sinf(m_timer * M_PI * 0.5 / m_timeToMove) + m_cameraStartingPoint);
	}

	if (m_timer >= m_timeToMove)
	{
		m_isCameraArrived = true;
	}
}

void CamData::InterpolCameraTo(sf::Vector2f _pos, float _timeTraveling)
{
	m_timeToMove = _timeTraveling;
	m_timer = 0;
	m_isCameraArrived = false;
	m_cameraFinalPoint = _pos;
	m_cameraStartingPoint = GetPosition();

	//We put the offset of the camera to 0 and the traking of an object deactivated so it go exactly where it should
	m_cameraOffset = { 0,0 };
	m_shouldCameraFollowObject = false;
}

void CamData::ShakeCamera(unsigned int _intensity, float _speedShake, float _shakeTimer)
{
	m_intensity = _intensity;
	m_baseShakeSpeed = _speedShake;
	m_shakeSpeed = _speedShake;
	m_shakeTimer = 1 / m_shakeSpeed;
	m_shakeTimeLeft = _shakeTimer;
	m_timeToShake = _shakeTimer;
}

bool CamData::IsShaking()
{
	return (m_shakeTimeLeft > 0) ? true : false;
}

#pragma region Constructor & destructor

CamData::CamData()
{
	m_camera.setSize({ SCREEN_WIDTH , SCREEN_HEIGHT });
	m_camera.setCenter({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
}

CamData::~CamData()
{

}

#pragma endregion

#pragma region Setter

void CamData::SetCamera(sf::View _camera)
{
	m_camera = _camera;
}

void CamData::SetPosition(sf::Vector2f _pos)
{
	m_camera.setCenter(_pos + m_cameraOffset);
}

void CamData::SetPositionB2D(b2Vec2 _pos)
{
	sf::Vector2f posScreen = World2Screen(_pos, { 0,0 });
	m_camera.setCenter(posScreen);
}

void CamData::SetOffset(sf::Vector2f _offset)
{
	m_cameraOffset = _offset;
}

void CamData::SetZoom(float _zoom)
{
	m_camera.zoom(_zoom);
}

void CamData::SetCameraRange(sf::Vector2f _rangeX, sf::Vector2f _rangeY)
{
	m_cameraRange = { _rangeX, _rangeY };
}

void CamData::SetCameraRangeX(sf::Vector2f _rangeX)
{
	m_cameraRange.first = _rangeX;
}

void CamData::SetCameraRangeY(sf::Vector2f _rangeY)
{
	m_cameraRange.second = _rangeY;
}

void CamData::SetCameraFollowPlayer(bool _doFollow)
{
	m_shouldCameraFollowObject = _doFollow;
}

void CamData::SetCameraGoBackOrigin(bool _bool)
{
	m_shouldCameraGoBackToOrigin = _bool;
}

#pragma endregion

#pragma region Getter

sf::View CamData::GetCamera()
{
	return m_camera;
}

sf::Vector2f CamData::GetPosition()
{
	return m_camera.getCenter();
}

sf::Vector2f CamData::GetPositionB2D()
{
	b2Vec2 camPos = Screen2World(m_camera.getCenter() + m_cameraOffset, { 0,0 });
	return sf::Vector2f(camPos.x, camPos.y);
}

sf::Vector2f CamData::GetOffset()
{
	return m_cameraOffset;
}

sf::Vector2f CamData::GetScale()
{
	sf::Vector2f camSize = m_camera.getSize();
	return sf::Vector2f{ camSize.x / SCREEN_WIDTH, camSize.y / SCREEN_HEIGHT };
}

std::pair<sf::Vector2f, sf::Vector2f> CamData::GetCameraRange()
{
	return m_cameraRange;
}

sf::Vector2f CamData::GetCameraRangeX()
{
	return m_cameraRange.first;
}

sf::Vector2f CamData::GetCameraRangeY()
{
	return m_cameraRange.second;
}

bool CamData::GetCameraMoveStatus()
{
	return m_isCameraArrived;
}

bool CamData::GetCameraFollowPlayer()
{
	return m_shouldCameraFollowObject;
}

bool CamData::GetCameraGoBackOrigin()
{
	return m_shouldCameraGoBackToOrigin;
}

float CamData::GetZoom()
{
	return m_zoom;
}

#pragma endregion