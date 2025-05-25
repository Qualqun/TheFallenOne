#ifndef COMMON__H
#define COMMON__H

#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include "SFML/OpenGL.hpp"
#include "box2d/box2d.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define WINDOW_NAME "The Fallen One"
#define FPS_MAX 60

#define ZOOM 64
#define BOSS_PANDEMY_SPAWN_X 1500
#define BOSS_PANDEMY_SPAWN_Y -500
#define SPEED_ARROW_FLASHES 1.0f

#define GLYPH_PER_SECOND 15
#define SECOND_LASTING_TEXT 2
enum TypeShape
{
    CIRCLE,
    POLYGON
};
enum EntityCategory //To make more mask the value of it need to be 2x higher than the previous one
{
    BOUNDARY = 0x0001,
    BOSS = 0x0002,
    PLAYER = 0x0004,
    BALL = 0x0008,
    PIC = 0x0016,
    //THING = 0x0020,
    //THING = 0x0040,
    //THING = 0x0080,
    //THING = 0x0100,
    //THING = 0x0200,
    //THING = 0x0400,
    //THING = 0x0800,
    //THING = 0x1000,
    //THING = 0x2000,
    //THING = 0x4000,
    //THING = 0x8000,
};

struct SizePolygone
{
    float width;
    float height;
};

struct UserData
{
    std::string fixtureName = "Default Name";
    std::string partOfMap = "Is not a part of map";
    SizePolygone sizePolygone;

    //Grapling
    bool interruptGrapin = true;


    // Famine paterns
    void* animTears;
    void* animFlufekey;
    std::vector<b2Body*>* tearsGroundVec;
    void* seedGrowVec;
    void* flufekeyVec;

    bool canHit = true;

    //Pandemic
    void* m_ratShootVec;
};


struct B2Dinfo
{
    UserData userData; // do nothing if you dont want aply
    b2BodyType type;
    TypeShape shape;
    sf::Vector2f pos;
    std::vector<b2Vec2> points; //if you do a circle dont need to complete them

    bool fixedRotation = false; // false per default

    float radius;
    float density;
    float friction;
};

struct TextInfo
{
    sf::Text m_text;
    std::string m_string = "No Name";
    sf::Font m_font;
    float m_size;
    sf::Vector2f m_screenPos;
    sf::Vector2f m_pos;
    sf::Color m_color;
    float m_totalTime;
    float m_currentTime;
};

b2Body* GetBodyFromWorld(std::string _nameFixture, b2World* _world);         
b2Body* GetBodyOfMap(b2World* _world, std::string _elementName);

b2Body* CreateBodyB2D(b2World* _world, B2Dinfo _b2dInfo);
b2Body* CreateBodyB2D(b2World* _world, B2Dinfo _b2dInfo, uint16 _categoryBits, uint16 _maskBits);

sf::Sprite LoadSprite(const std::string _path, bool _isCentered);
sf::SoundBuffer LoadSoundBuffer(const std::string _path);

void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window);
void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window, int _transparency);
void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window, sf::Shader& _shader);
void BlitText(sf::Text _text, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window);

sf::Vector2f World2Screen(b2Vec2 _worldPos, sf::Vector2f _cam);
sf::Vector2f World2Screen(b2Vec2 _worldPos);
sf::Vector2f World2ScreenVec(sf::Vector2f _screenPos);


float Pixel2Meter(float _pixels);
float Meter2Pixel(float _meters);

b2Vec2 Screen2World(sf::Vector2f _screenPos, sf::Vector2f _cam);
b2Vec2 Screen2World(sf::Vector2f _screenPos);
sf::Vector2f Screen2WorldVec(sf::Vector2f _screenPos);

bool CollideCirclePoint(b2Vec2 _circlePos, float _radius, b2Vec2 _point);
b2Body* CreateProjectile(b2World* _world, float _radius, float _density, float _speed, sf::Vector2f _pos);
b2Body* CreateProjectile(b2World* _world, float _radius, float _density, float _speed, sf::Vector2f _pos, std::string _name);
bool isBodyOnScreen(b2Body* _body);

// \brief Covert a angle in degree to a angle in radian
// \param A degree angle
// \return A radian angle
float DegToRad(double _angle);
// \brief Covert a angle in radian to a angle in degree
// \param A radian angle
// \return A degree angle
float RadToDeg(double _angle);

// \brief Convert a directional vector to a unit vector
// \param A directional vector
// \return A unit vector
sf::Vector2f VecDirToVecUnit(sf::Vector2f _vectorDir);

#endif