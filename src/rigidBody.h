#pragma once

#include <ogre.h>
#include <OgreNewt.h>

using namespace Ogre;
using namespace OgreNewt;

extern void gravityForceCallback( OgreNewt::Body* me );

class RigidBody
{
public:

	//scene node pointer
	SceneNode *mNode; 

	//physics body pointer
	Body *mBody; 

	RigidBody();
	~RigidBody();

	//returns scene node position
	Vector3 getPosition();

	//returns scene node orientation
	Quaternion getOrientation();

	//sets position of physics body
	void setPosition(Vector3 pos);

	//sets orientation of physics body
	void setOrientation(Quaternion orient);

	//sets position / orientation of physics body
	void setPositionOrientation (Vector3 pos, Quaternion orient);

	//stop the body from moving and rotating
	void stopBody();

	//returns true if the body is frozen
	bool isBodySleeping() {return mBody->getSleepingState() == 0; }

};

