#include "..\\globals.h"
#include "puck.h"
#include "physicsmat.h"

void Puck::puckForceCallback( Body* me )
{
	//apply a simple gravity force.
	gravityForceCallback(me);

	//restrict to max velocity
	Vector3 vel = me->getVelocity();
	if (vel.squaredLength() > PUCK_MAX_VEL_SQR)
	{
		vel.normalise();
		vel = vel * PUCK_MAX_VEL;
		me->setVelocity(vel);
	}
}


void Puck::puckTransformCallback(Body *me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos)
{
	Quaternion newOrient;
	Vector3 newPos = pos;

	//clamp Y coordinate
	if ( ((Puck*)me->getUserData())->getClampY() != -1)
		newPos.y = ((Puck*)me->getUserData())->getClampY();

	newOrient = newOrient * Quaternion(orient.getYaw(), Vector3::UNIT_Y);
	
	//set new body position and orientation
	me->setPositionOrientation(newPos, newOrient);

	//set new node position and orientation
	me->getOgreNode()->setPosition(newPos);
	me->getOgreNode()->setOrientation(newOrient);	
}


Puck::Puck()
{
	mCollision = NULL; 
	mClampY = -1;
}

Puck::~Puck()
{
	if (mCollision) delete mCollision;
}

void Puck::init (SceneManager *scnMgr, String sceneNodeName, World *world, Real cylRadius, Real cylHeight)
{
	//copy scene node
	mNode = scnMgr->getSceneNode(sceneNodeName);

	//rotate cylinder to match puck orientation
	Quaternion orient = Quaternion(Radian(TORADIANS(90)),Vector3::UNIT_Z);

	//create collision object
	mCollision = new CollisionPrimitives::Cylinder(world, cylRadius, cylHeight, orient, Vector3(0,cylHeight/2.0, 0));

	//compute inertia
	Vector3 inertia = MomentOfInertia::CalcCylinderSolid(PUCK_MASS, cylRadius, cylHeight);
	
	//initialize body
	mBody = new Body(world, mCollision);
	mBody->setUserData(this);
	mBody->setMassMatrix(PUCK_MASS, inertia);
	mBody->attachToNode(mNode);
	mBody->setMaterialGroupID(PhysicsMat::getSingletonPtr()->getPuckMatID());
	mBody->setLinearDamping(0);

	//set appropriate callbacks
	mBody->setCustomForceAndTorqueCallback(puckForceCallback);
	mBody->setCustomTransformCallback(puckTransformCallback);
}

//used by the ai component to move this body
void Puck::moveTo (Real x, Real y, Real z, Real speed)
{
	Vector3 pos = mNode->getPosition();
	Vector3 vel;

	vel.x = x - pos.x;
	vel.y = y - pos.y;
	vel.z = z - pos.z;
	vel.normalise();
	vel *= speed;
	mBody->setVelocity(vel);	
}

//used by the ai component to move this body
void Puck::moveTo (Vector3 pos, Real speed)
{
	moveTo (pos.x, pos.y, pos.z, speed);
}