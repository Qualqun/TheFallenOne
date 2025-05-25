#ifndef MAP_H
#define MAP_H

#include "Animation.hpp"
#include "Background.hpp"
#include "Particle.hpp"
#include "Sound.hpp"

#define SIZE_ROOF_FAMINE 25

#define SKULL_WIDTH 415
#define SKULL_HEIGHT 255
#define SKULL_ACTIVATION_RANGE 400.f

enum ElementMap
{
	GROUND,
	WALL,
};

enum MapStatus
{
	NOTHING,
	SPAWN_PANDEMIC,
	SPAWN_FAMINE,
	LOAD_SECOND_MAP_FAMINE,
	SPAWN_WAR,
	SPAWN_DEATH,
	SPAWN_JUMP_LEVEL
};

enum SkullAnimations
{
	APPARITION_ANIM,
	LOADING_ANIM,
	TOTAL_SKULLANIM
};
class MapData
{
public:
	MapData();
	MapData(b2World* _world, BackgroundData& _background, int _nbBoss, SoundData& _sound);

	void Update(b2Body* _player, b2World* _world, float _dt, ParticleData& _particle);
	void Draw(sf::RenderWindow& _window);
	b2Vec2 SearchHook(b2Vec2 _posGrapling);

	void SetLoading(bool _statusLoading) { m_loading = _statusLoading; }
	void SetStatus(int _status) { m_status = _status; }
	int GetStatus() { return m_status; }
	int GetNBBossMap() { return m_nbBossMap; }

	void LoadMapFamine(b2World* _world, BackgroundData& _background, SoundData& _sound);
	void LoadSecondMapFamine(b2World* _world, BackgroundData& _background);

	void LoadMapPandemic(b2World* _world, BackgroundData& _background, SoundData& _sound);
	void LoadMapWar(b2World* _world, BackgroundData& _background);
	void LoadMapDeath(b2World* _world, BackgroundData& _background, SoundData& _sound);
	void LoadMapJump(b2World* _world, BackgroundData& _background, SoundData& _sound);
	void DestroyMap(b2World* _world);

	bool CheckDistancePlayerSkull(b2World* _world);


	~MapData() {};
protected:

	void CreateSkull(sf::Vector2f _pos, b2Vec2 _size, b2World* _world);
	void CreateHook(sf::Vector2f _pos, b2World* _world);
	void CreatePlatform(sf::Vector2f _pos, b2Vec2 _size, b2World* _world, std::string _partOfMap);
	void CreateWall(sf::Vector2f _pos, b2Vec2 _size, b2World* _world, std::string _partOfMap);

private:

	std::vector<b2Body*> m_ground;
	std::vector<b2Body*> m_wall;
	std::vector<b2Body*> m_hook;
	b2Body* m_skull;


	std::map<std::string, int> m_groundTablePos;
	std::map<std::string, int> m_wallTablePos;
	std::map<std::string, int> m_hookTablePos;


	bool m_skullAnimBlocked;
	bool m_skullIsFinish;
	bool m_loading;
	b2Vec2 m_playerSpawnPos;
	std::vector<std::pair<float, std::string>> m_trigerX;
	int m_status;
	int m_nbBossMap;

	sf::Sprite m_hookSprite;
	sf::Sprite m_skullSprite;


	Anim m_skullAnim[TOTAL_SKULLANIM];
	SkullAnimations m_typeAnims;
};


#endif