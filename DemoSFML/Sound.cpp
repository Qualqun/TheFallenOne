#include "Sound.hpp"

//Load to memory all sound, use of .ogg for a more compressed sound than .wav
void SoundData::InitSound()
{
	SetVolume(VOLUME_DEFAULT);

	CreateSoundSpace(SOURCE_MENU, TOTAL_MENU_SOUND);
	soundBuffer[SOURCE_MENU][BUTTON_CLICK].loadFromFile("Sounds/Menu/ButtonClick.ogg");

	//Player sound load
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Attack/Hit1.ogg"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Attack/Hit2.ogg"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Attack/Hit3.ogg"));

	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Attack/NoHit1.ogg"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Attack/NoHit2.ogg"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Attack/NoHit3.ogg"));

	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Footsteps/Dirt1.wav"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Footsteps/Dirt2.wav"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/Footsteps/Dirt3.wav"));

	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/HitHook.ogg"));
	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/SendHook.ogg"));

	soundBuffer[SOURCE_PLAYER].push_back(LoadSoundBuffer("Sounds/Player/BodyHit.wav"));


	//Boss pandemic

	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/SpawnPandemic.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/LunchBoss.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/Wind.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/mnstr1.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/mnstr2.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/mnstr3.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/mnstr4.wav"));
	soundBuffer[SOURCE_BOSS_PANDEMIC].push_back(LoadSoundBuffer("Sounds/Pandemic/mnstr5.wav"));
}

//Play a sound (enum with all sound id in sound.hpp)
void SoundData::UseSound(unsigned int _soundSourceId, unsigned int _soundId)
{
	size_t i = 0;
	//Pas le droit d'utiliser des breaks dans un while d'après les règles de codage mais tkt, je les ai changer par des returns
	while (true)
	{
		if (i == SOUND_CANAL_NUMBER)
		{
			std::cout << "Can't play sound. No sound canal left !" << std::endl;
			return;
		}
		if (sound[i].getStatus() == sf::Sound::Stopped)
		{
			sound[i].setBuffer(soundBuffer[_soundSourceId][_soundId]);
			sound[i].play();
			return;
		}
		i++;
	}
}

//Necesary to call before starting to assign sound buffer to the vector
void SoundData::CreateSoundSpace(unsigned int _soundSource, unsigned int _nbTotalSound)
{
	soundBuffer[_soundSource].resize(_nbTotalSound);
}

//Start the background music and make it loop
void SoundData::StartBackgroundMusic(unsigned int _musicId)
{
	music.stop();
	switch (_musicId)
	{
	case MAIN_MENU:
		music.openFromFile("Sounds/Music/MainMenu.ogg");
		music.play();
		break;
	case MUSIC_DEATH:
		music.openFromFile("Sounds/Music/Death.ogg");
		music.play();
		break;
	case MUSIC_FAMINE:
		music.openFromFile("Sounds/Music/Famine.ogg");
		music.play();
		break;
	case MUSIC_PANDEMIC:
		music.openFromFile("Sounds/Music/Pandemic.ogg");
		music.play();
		break;
	default:
		music.stop();
		break;
	}
	music.setLoop(true);
}


//Set the volume, the value should range between 0(min) and 100(max)
void SoundData::SetVolume(float _volume)
{
	if (_volume > 100)
	{
		_volume = 100;
	}
	if (_volume < 0)
	{
		_volume = 0;
	}
	volume = _volume;
	ApplyVolume();
}

//Apply the volume to all sound chanel
void SoundData::ApplyVolume()
{
	for (size_t i = 0; i < SOUND_CANAL_NUMBER; i++)
	{
		sound[i].setVolume(volume);
	}
	music.setVolume(volume);
}

SoundData::SoundData()
{
	InitSound();
}

SoundData::~SoundData()
{

}