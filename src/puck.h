#pragma once

#include "rigidbody.h"

using namespace Ogre;
using namespace OgreNewt;

class Puck : public RigidBody
{
protected:

	//pointer to collision object
	CollisionPrimitives::Cylinder *mCollision;

	//this flag is set when puck collides with a paddle
	bool mIsCollideToPaddle;

	//y coordinate where puck will sit
	Real mClampY;

	//force callback to apply gravity
	static void puckForceCallback( Body* me );

	//called when the body moves
	static void puckTransformCallback(Body *me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos);

public:
	Puck();
	~Puck();

	//initializes physics body
	void init (SceneManager *scnMgr, String sceneNodeName, World *world, Real cylRadius, Real cylHeight);

	//used by the ai component to move this body
	void moveTo (Real x, Real y, Real z, Real speed);
	void moveTo (Vector3 pos, Real speed);

	//set/get the mIsCollideToPaddle flag
	void setIsCollideToPaddle (bool isCollideToPaddle) { mIsCollideToPaddle = isCollideToPaddle;};
	bool getIsCollideToPaddle () {return mIsCollideToPaddle;}

	//set/get mClampY
	void setClampY(Real y) {mClampY = y;}
	Real getClampY() {return mClampY;}
};