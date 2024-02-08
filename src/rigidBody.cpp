#include "rigidbody.h"
#include "globals.h"

void gravityForceCallback( OgreNewt::Body* me )
{
	//apply a simple gravity force.
	Real mass;
	Vector3 inertia;
	Vector3 force(0,GRAVITY,0);
	me->getMassMatrix(mass, inertia);
	force *= mass;
	me->addForce( force );
}


RigidBody::RigidBody()
{
	mNode = NULL;
	mBody = NULL;

}

RigidBody::~RigidBody()
{
}

//returns scene node position
Vector3 RigidBody::getPosition()
{
	return mNode->getPosition();
}

//returns scene node orientation
Quaternion RigidBody::getOrientation()
{
	return mNode->getOrientation();
}

//sets position of physics body
void RigidBody::setPosition(Vector3 pos)
{
	mBody->setPositionOrientation(pos, getOrientation());
}

//sets orientation of physics body
void RigidBody::setOrientation(Quaternion orient)
{
	mBody->setPositionOrientation(getPosition(), orient);
}

//sets position / orientation of physics body
void RigidBody::setPositionOrientation (Vector3 pos, Quaternion orient)
{
	mBody->setPositionOrientation(pos, orient);
}

//stop the body from moving and rotating
void RigidBody::stopBody()
{
	mBody->setVelocity(Vector3(0,0,0));
	mBody->setOmega(Vector3(0,0,0));
}