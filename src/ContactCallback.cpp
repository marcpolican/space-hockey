#pragma once

#include "contactcallback.h"
#include "soundmanager.h"


CallbackPuckPaddle::CallbackPuckPaddle()
{
	mIsCollide = false;
}

int CallbackPuckPaddle::userProcess()
{
	//check if there is a collision velocity, if none no collision has taken place
	collisionVel = getContactNormalSpeed();
	if (collisionVel > 0)
		mIsCollide = true;

	return ContactCallback::userProcess();
}
void CallbackPuckPaddle::userEnd()
{
	if (!mIsCollide) return;

	//play sound
	Real volume = collisionVel * 0.1;
	if (volume > 1.0) 
		volume = 1.0;
	
	SoundManager::getSingletonPtr()->playHit(volume);
	mIsCollide = false;
}



CallbackPuckWall::CallbackPuckWall()
{
	mIsCollide = false;
}

int CallbackPuckWall::userProcess()
{
	//check if there is a collision velocity, if none no collision has taken place
	collisionVel = getContactNormalSpeed();
	if (collisionVel > 0)
		mIsCollide = true;

	return ContactCallback::userProcess();
}
void CallbackPuckWall::userEnd()
{
	if (!mIsCollide) return;

	//play sound
	Real volume = collisionVel * 0.1;
	if (volume > 1.0) 
		volume = 1.0;
	
	SoundManager::getSingletonPtr()->playHit(volume);
	mIsCollide = false;
}


