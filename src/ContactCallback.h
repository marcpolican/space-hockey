#pragma once

#include <ogre.h>
#include <ogrenewt.h>

using namespace Ogre;
using namespace OgreNewt;


//puck - paddle collision callback class
class CallbackPuckPaddle : public ContactCallback
{
	bool mIsCollide;
	Real collisionVel;

public:
	CallbackPuckPaddle();
	int userProcess();
	void userEnd();
};

//puck - table wall collision callback class
class CallbackPuckWall : public ContactCallback
{
	bool mIsCollide;
	Real collisionVel;

public:
	CallbackPuckWall();
	int userProcess();
	void userEnd();
};
