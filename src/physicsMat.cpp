#include "..\\globals.h"
#include "PhysicsMat.h"


template<> PhysicsMat* Singleton<PhysicsMat>::ms_Singleton = 0;

PhysicsMat::PhysicsMat()
{
	mPuck = NULL;
	mPaddle = NULL;
	mTableWall = NULL;
	mTableSurface = NULL;

	mPuckPaddle = NULL;
	mPuckTableWall = NULL;
	mPaddleTableWall = NULL;

	mPuckTableSurface = NULL;
	mPaddleTableSurface = NULL;

	mCallback = new CallbackPuckPaddle;
}

PhysicsMat::~PhysicsMat()
{
	if (mPuck) delete mPuck;
	if (mPaddle) delete mPaddle;
	if (mTableWall) delete mTableWall;
	if (mTableSurface) delete mTableSurface;

	if (mPuckPaddle) delete mPuckPaddle;
	if (mPuckTableWall) delete mPuckTableWall;
	if (mPaddleTableWall) delete mPaddleTableWall;

	if (mPuckTableSurface) delete mPuckTableSurface;
	if (mPaddleTableSurface) delete mPaddleTableSurface;

	if (mCallback) delete mCallback;
}

void PhysicsMat::init (OgreNewt::World *world)
{
	//create material ids
	mPuck = new MaterialID(world);
	mPaddle = new MaterialID(world);
	mTableWall = new MaterialID(world);
	mTableSurface = new MaterialID(world);

	//create material pair for puck and paddle
	mPuckPaddle = new MaterialPair    (world, mPuck, mPaddle);
	mPuckPaddle->setContactCallback(mCallback);
	mPuckPaddle->setDefaultElasticity (0.95);
	mPuckPaddle->setDefaultSoftness   (1.0);

	//create material pair for puck and wall
	mPuckTableWall = new MaterialPair     (world, mPuck, mTableWall);
	mPuckTableWall->setContactCallback(mCallback);
	mPuckTableWall->setDefaultFriction    (0.0, 0.0);
	mPuckTableWall->setDefaultElasticity  (0.9);

	//create material pair for paddle table wall
	mPaddleTableWall = new MaterialPair   (world, mPaddle, mTableWall);
	mPaddleTableWall->setDefaultElasticity (0.0);

	//create material pair for puck and table surface
	mPuckTableSurface = new MaterialPair    (world, mPuck, mTableSurface);
	mPuckTableSurface->setDefaultFriction   (0.1, 0.05);
	mPuckTableSurface->setDefaultElasticity (0.1);

	//create material pair for paddle and table surface
	mPaddleTableSurface = new MaterialPair    (world, mPaddle, mTableSurface);
	mPaddleTableSurface->setDefaultFriction   (0.1, 0.1);
	mPaddleTableSurface->setDefaultElasticity (0.0);

}