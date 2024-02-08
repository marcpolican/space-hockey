#include "..\\globals.h"
#include "paddle.h"
#include "physicsmat.h"


void Paddle::paddleForceCallback( Body* me )
{
	Paddle *paddle;
	Real mass, timestep;
	Vector3 inertia, desiredVel, currentVel;

	paddle = (Paddle*) me->getUserData();

	//check if this paddle is controlled by the player
	if (paddle->isPlayerControlled())
	{
		desiredVel = paddle->getMoveOffset();
		if (desiredVel.isZeroLength())
		{
			//stop paddle
			me->setVelocity(Vector3 (0,0,0));
		}
		else
		{
			//add a force depending on the velocities
			me->getMassMatrix(mass, inertia);
			currentVel = me->getVelocity();
			timestep = me->getWorld()->getTimeStep();
			

			Vector3 force = (mass) * (( desiredVel - currentVel) / (timestep) ); 
			force.y = 0; //dont add vertical force

			me->addForce(force);

		}
	}

	//limit paddle speed
	Vector3 vel = me->getVelocity();
	if (vel.squaredLength() > PADDLE_MAX_VEL_SQR)
	{
		vel.normalise();
		vel = vel * PADDLE_MAX_VEL;
		me->setVelocity(vel);
	}
}

void Paddle::paddleTransformCallback(Body *me, const Quaternion& orient, const Vector3& pos)
{
	Vector3 newPos = pos;
	Quaternion newOrient = Quaternion::IDENTITY;
	bool bChanged = false;
	Paddle *pad = (Paddle*)me->getUserData();

	//check if paddle went out of bounds
	if (newPos.x < -TABLE_DIM_X_HALF + PADDLE_RADIUS) 
	{
		newPos.x = -TABLE_DIM_X_HALF + PADDLE_RADIUS;
		bChanged = true;
	}

	if (newPos.x > TABLE_DIM_X_HALF - PADDLE_RADIUS)
	{
		newPos.x = TABLE_DIM_X_HALF - PADDLE_RADIUS;
		bChanged = true;
	}
	
	if (newPos.z < pad->getBoundsZLow() + PADDLE_RADIUS) 
	{
		newPos.z = pad->getBoundsZLow() + PADDLE_RADIUS;
		bChanged = true;
	}

	if (newPos.z > pad->getBoundsZHigh() - PADDLE_RADIUS)
	{
		newPos.z = pad->getBoundsZHigh() - PADDLE_RADIUS;
		bChanged = true;
	}

	//clamp paddle to this Y coordinate
	if ( ((Paddle*)me->getUserData())->getClampY() != -1)
		newPos.y = ((Paddle*)me->getUserData())->getClampY();

	//change position and orientation of the body
	me->setPositionOrientation(newPos, newOrient);

	//set position and orientation of scene node
	me->getOgreNode()->setPosition(newPos);
	me->getOgreNode()->setOrientation(newOrient);
}

Paddle::Paddle()
{
	mCollision = NULL;
	mClampY = -1;

	mIsPlayerControlled = false;
}

Paddle::~Paddle()
{
	if (mCollision) delete mCollision;
}


void Paddle::init (SceneManager *scnMgr, String sceneNodeName, World *world, Real cylRadius, Real cylHeight)
{
	//save scene node pointer
	mNode = scnMgr->getSceneNode(sceneNodeName);

	//rotate cylinder to match paddle orientation
	Quaternion orient = Quaternion(Radian(TORADIANS(90)),Vector3::UNIT_Z);

	//create collision object
	mCollision = new CollisionPrimitives::Cylinder(world, cylRadius, cylHeight, orient, Vector3(0,cylHeight/2.0, 0));

	//calculate inertia
	Vector3 inertia = MomentOfInertia::CalcCylinderSolid(PADDLE_MASS, cylRadius, cylHeight);
	
	//initialize physics body
	mBody = new Body(world, mCollision);
	mBody->setUserData(this);
	mBody->setMassMatrix(PADDLE_MASS, inertia);
	mBody->attachToNode(mNode);
	mBody->setMaterialGroupID(PhysicsMat::getSingletonPtr()->getPaddleMatID());

	//set appropriate callbacks
	mBody->setCustomForceAndTorqueCallback(paddleForceCallback);
	mBody->setCustomTransformCallback(paddleTransformCallback);
} 

void Paddle::move (Vector3 offset)
{
	//this will be used in the paddleForceCallback function
	mMoveOffset = offset;
}

void Paddle::setBoundsZ (Real zLow, Real zHigh)
{
	//added checking
	if (zLow < zHigh)
	{
		mBoundsZLow = zLow;
		mBoundsZHigh = zHigh;
	}
	else
	{
		mBoundsZLow = zHigh;
		mBoundsZHigh = zLow;	
	}
}

//used by the ai component to move this body
void Paddle::moveTo (Real x, Real y, Real z, Real speed)
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
void Paddle::moveTo (Vector3 pos, Real speed)
{
	moveTo (pos.x, pos.y, pos.z, speed);
}