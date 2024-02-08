#pragma once

#include "rigidbody.h"

using namespace Ogre;
using namespace OgreNewt;

class Paddle : public RigidBody
{
protected:

	//collision object
	CollisionPrimitives::Cylinder *mCollision;

	//used in the force callback function
	Vector3 mMoveOffset;
	
	//range where the paddle can travel
	Real mBoundsZLow, mBoundsZHigh;

	//y coordinate where paddle will sit
	Real mClampY;

	//flag if this paddle is controlled by the mouse
	bool mIsPlayerControlled;

	//force callback to apply gravity and other forces
	static void paddleForceCallback( Body* me );

	//called when the body moves. this is used to limit where the paddle can travel
	static void paddleTransformCallback(Body *me, const Quaternion& orient, const Vector3& pos);

public:
	Paddle();
	~Paddle();

	void init (SceneManager *scnMgr, String sceneNodeName, World *world, Real cylRadius, Real cylHeight);
	void setBoundsZ (Real zLow, Real zHigh);
	void move (Vector3 offset);
	void moveTo (Real x, Real y, Real z, Real speed);
	void moveTo (Vector3 pos, Real speed);


	//return low z bound
	Real getBoundsZLow() {return mBoundsZLow;}

	//return high z bound
	Real getBoundsZHigh() {return mBoundsZHigh;}

	//returns mMoveOffset
	Vector3 getMoveOffset() {return mMoveOffset;}

	//set/get mClampY
	void setClampY(Real y) {mClampY = y;}
	Real getClampY() {return mClampY;}

	//set/get mIsPlayerControlled
	void setPlayerControlled(bool b) {mIsPlayerControlled = b;}
	bool isPlayerControlled() {return mIsPlayerControlled;}

};