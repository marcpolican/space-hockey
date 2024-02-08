//reference: http://www.ogre3d.org/wiki/index.php/Managing_Game_States_with_OGRE

#include "soundmanager.h"
#include "globals.h"

template<> SoundManager* Ogre::Singleton<SoundManager>::ms_Singleton = 0;

#ifndef NO_SOUNDS

SoundManager::SoundManager()
{
	mCurrentSource = 0;
	alutInit (0, 0);
}

SoundManager::~SoundManager()
{
	alutExit();
}


void SoundManager::initAllSounds()
{
	ALenum error;
	//init background music
	mBuffers[SND_BACKGROUND_MUSIC] = alutCreateBufferFromFile ("..\\media\\sounds\\music.wav");
	alGenSources (1, &mSourceMusic);
	alSourcei (mSourceMusic, AL_BUFFER, mBuffers[SND_BACKGROUND_MUSIC]);
	alSourcef (mSourceMusic, AL_GAIN, 0.5);
	alSourcei (mSourceMusic, AL_LOOPING, AL_TRUE);

	//sound effects
	alGenSources (MAX_SOUND_BUFFERS, mSources);
	error = alGetError();
	if (error != ALUT_ERROR_NO_ERROR)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage(alutGetErrorString(error));
	}

	//init sound effects
	mBuffers[SND_HIT] = alutCreateBufferFromFile ("..\\media\\sounds\\hit.wav");
	mBuffers[SND_GOAL] = alutCreateBufferFromFile ("..\\media\\sounds\\goal.wav");
	mBuffers[SND_CLICK] = alutCreateBufferFromFile ("..\\media\\sounds\\click.wav");

	//set buffers to different sources
	alSourcei (mSources[SND_HIT], AL_BUFFER, mBuffers[SND_HIT]);
	alSourcei (mSources[SND_GOAL], AL_BUFFER, mBuffers[SND_GOAL]);
	alSourcei (mSources[SND_CLICK], AL_BUFFER, mBuffers[SND_CLICK]);
}

//general method to play a sound buffer
void SoundManager::playSoundBuffer(SoundManager::SoundID id)
{
	alSourcef (mSources[id], AL_GAIN, 1.0);
	alSourcePlay (mSources[id]);
}

//plays the hit sound with a paramater for volume depending on how hard the collision was
void SoundManager::playHit(Ogre::Real volume)
{
	alSourcef (mSources[SND_HIT], AL_GAIN, (float) volume);
	alSourcePlay (mSources[SND_HIT]);
}

//play goal sound
void SoundManager::playGoal()
{
	playSoundBuffer(SND_GOAL);
}

//play click sound
void SoundManager::playClick()
{
	playSoundBuffer(SND_CLICK);
}


//start looping music
void SoundManager::startMusic()
{
	alSourcePlay (mSourceMusic);
}

//stop looping music
void SoundManager::stopMusic()
{
	alSourceStop (mSourceMusic);
} 

#else

//function definitions if sound is turned off

SoundManager::SoundManager(){}
SoundManager::~SoundManager(){}
void SoundManager::initAllSounds(){}
void SoundManager::playSoundBuffer(SoundManager::SoundID id){}
void SoundManager::playHit(Ogre::Real volume){}
void SoundManager::playGoal(){}
void SoundManager::playClick(){}
void SoundManager::startMusic(){}
void SoundManager::stopMusic(){} 

#endif