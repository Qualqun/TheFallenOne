#ifndef SOUND_HPP
#define SOUND_HPP

#include "Common.h"

#define SOUND_CANAL_NUMBER 127
#define VOLUME_DEFAULT 50.f

enum SoundSource
{
	SOURCE_MENU,
	SOURCE_PLAYER,

	SOURCE_BOSS_PANDEMIC,
	SOURCE_BOSS_WAR,
	SOURCE_BOSS_FAMINE,
	SOURCE_BOSS_DEATH,

	TOTAL_SOURCE_TYPE
};

enum MenuSoundType
{
	BUTTON_CLICK,
	BUTTON_HOVER,

	TOTAL_MENU_SOUND
};
enum PlayerSoundType
{
	HIT_ONE,
	HIT_TWO,
	HIT_THREE,

	NO_HIT_ONE,
	NO_HIT_TWO,
	NO_HIT_THREE,

	DIRT_ONE,
	DIRT_TWO,
	DIRT_THREE,

	HIT_HOOK,
	SEND_HOOK,

	TAKE_DAMAGE,

	TOTAL_PLAYER_SOUND
};

enum PandemicSoundType
{
	SPAWN_BOSS_SOUND,
	LAUNCH_BOSS_SOUND,
	WIND_SOUND,

	ATTACK_SOUND_1,
	ATTACK_SOUND_2,
	ATTACK_SOUND_3,
	ATTACK_SOUND_4,
	ATTACK_SOUND_5,

};

enum MusicId
{
	MAIN_MENU,
	MUSIC_DEATH,
	MUSIC_FAMINE,
	MUSIC_PANDEMIC,

	TOTAL_MUSIC
};

class SoundData
{
private:

	float volume;

	size_t testSoundIndex = 0;

	void ApplyVolume();

public:
	std::vector<sf::SoundBuffer> soundBuffer[TOTAL_SOURCE_TYPE];
	sf::Sound sound[SOUND_CANAL_NUMBER];
	sf::Music music;

	SoundData();
	~SoundData();

	void InitSound();
	void StartBackgroundMusic(unsigned int _musicId);
	void SetVolume(float _volume);
	float GetVolume() { return volume; }
	size_t GetTestSoundIndex() { return testSoundIndex; }
	void IncrementTestSoundIndex()
	{
		testSoundIndex++;
		if (testSoundIndex == TOTAL_SOURCE_TYPE)
		{
			testSoundIndex = 0;
		}
	}
	void UseSound(unsigned int _soundSourceId, unsigned int _soundId);
	void CreateSoundSpace(unsigned int _soundSource, unsigned int _nbTotalSound);
};



#endif