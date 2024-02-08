#pragma once

#include "rigidbody.h"

using namespace Ogre;
using namespace OgreNewt;

class Table : public RigidBody
{
protected:
	CollisionPrimitives::TreeCollisionSceneParser *mCollision;

public:
	Table();
	~Table();

	//initializes physics body
	void init (SceneManager *scnMgr, String sceneNodeName, World *world,MaterialID *matID);
};