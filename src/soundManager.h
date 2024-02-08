#pragma once

#include <ogre.h>
#include <alut.h>

class SoundManager : public Ogre::Singleton<SoundManager>
{
protected:

	enum SoundID
	{
		SND_BACKGROUND_MUSIC,
		SND_HIT,
		SND_GOAL,
		SND_CLICK,
		MAX_SOUND_BUFFERS
	};

	ALuint mSources[MAX_SOUND_BUFFERS];
	ALuint mBuffers[MAX_SOUND_BUFFERS];

	ALuint mSourceMusic;
	ALuint mSourceTimeLow;


	int mCurrentSource;

	void playSoundBuffer(SoundManager::SoundID id);

public:

	SoundManager();
	virtual ~SoundManager();

	static SoundManager& getSingleton()    { assert(ms_Singleton); return *ms_Singleton; }
	static SoundManager* getSingletonPtr() { return ms_Singleton; }

	void initAllSounds();
	void playHit(Ogre::Real volume = 1.0);

	void playGoal();
	void playClick();

	void startMusic();
	void stopMusic();
};

