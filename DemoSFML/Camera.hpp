#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common.h"

#define CAMERA_OFFSET_PLAYER_X 480
#define CAMERA_OFFSET_PLAYER_Y -270

#define CAM_SPEED 500

class CamData
{
private:

	//ALL COORD ARE SCREEN COORD ! (not box2d cord)

	sf::View m_camera;
	sf::Vector2f m_cameraOffset = { 0,0 };

	//The first element of the pair is the x range, the second element of the pair is the y range
	//In the vector the first element is the minimum and the second the maximum
	std::pair<sf::Vector2f, sf::Vector2f> m_cameraRange = { { (float)INT_MIN,(float)INT_MAX} ,{ (float)INT_MIN,(float)INT_MAX} };

	int m_intensity = 0;
	float m_shakeSpeed = 0;
	float m_baseShakeSpeed = 0;
	float m_shakeTimer = 0;
	float m_shakeTimeLeft = 0;
	float m_timeToShake = 0;

	sf::Vector2f m_cameraStartingPoint;
	sf::Vector2f m_cameraFinalPoint;
	bool m_isCameraArrived = true;
	float m_timer = 0;
	float m_timeToMove = 0;
	float m_timeToWait = 0;
	bool m_shouldCameraGoBackToOrigin = false;

	bool m_shouldCameraFollowObject = true;

	float m_zoom = 1.f;

public:
	CamData();
	~CamData();

	void MoveCamera(sf::Vector2f _pos);
	void MouseWheelScrolledCamera(sf::RenderWindow& _window, sf::Event& _event);

	void InterpolCameraTo(sf::Vector2f _pos, float _timeTraveling);
	void MoveCameraSmooth(float _dt);

	// \brief Make the camera shake for a period of time
	// \param The number of pixel it can reach (can go out of the camera range if set too high)
	// \param The number of shake in 1 second
	// \param The time it shake
	void ShakeCamera(unsigned int _intensity, float _speedShake, float _shakeTimer);
	// \return True if the time to shake is superior than 0, else false
	bool IsShaking();

	//Setter
	void SetCamera(sf::View _camera);

	void SetPosition(sf::Vector2f _pos);
	void SetPositionB2D(b2Vec2 _pos);

	void SetOffset(sf::Vector2f _offset);
	void SetZoom(float _zoom);

	void SetCameraRange(sf::Vector2f _rangeX, sf::Vector2f _rangeY);
	void SetCameraRangeX(sf::Vector2f _rangeX);
	void SetCameraRangeY(sf::Vector2f _rangeY);

	void SetCameraFollowPlayer(bool _doFollow);
	void SetCameraGoBackOrigin(bool _bool);

	//Getter
	sf::View GetCamera();

	sf::Vector2f GetPosition();
	sf::Vector2f GetPositionB2D();

	sf::Vector2f GetOffset();
	sf::Vector2f GetScale();

	std::pair<sf::Vector2f, sf::Vector2f> GetCameraRange();
	sf::Vector2f GetCameraRangeX();
	sf::Vector2f GetCameraRangeY();

	bool GetCameraMoveStatus();
	bool GetCameraFollowPlayer();
	bool GetCameraGoBackOrigin();

	float GetZoom();

	void Update(float _dt, sf::Vector2f _posTargetFollow);
};

#endif