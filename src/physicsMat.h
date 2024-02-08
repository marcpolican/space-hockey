#pragma once

#include <ogre.h>
#include <ogrenewt.h>
#include "ContactCallback.h"

using namespace Ogre;
using namespace OgreNewt;

class PhysicsMat : public Ogre::Singleton<PhysicsMat>
{
protected:
	//material id's
	MaterialID *mPaddle;
	MaterialID *mPuck;
	MaterialID *mTableWall;
	MaterialID *mTableSurface;

	//material pairs
	MaterialPair *mPuckPaddle;
	MaterialPair *mPuckTableWall;
	MaterialPair *mPaddleTableWall;
	MaterialPair *mPuckTableSurface;
	MaterialPair *mPaddleTableSurface;

	ContactCallback *mCallback;

public:
	PhysicsMat();
	~PhysicsMat();

	//initialize materials
	void init (OgreNewt::World *world);

	//return requested material id
	MaterialID* getPuckMatID()			{return mPuck;}
	MaterialID* getPaddleMatID()		{return mPaddle;}
	MaterialID* getTableWallMatID()		{return mTableWall;}
	MaterialID* getTableSurfaceMatID()  {return mTableSurface;}

	static PhysicsMat& getSingleton(void)    {assert (ms_Singleton); return *ms_Singleton;}
	static PhysicsMat* getSingletonPtr(void) {assert (ms_Singleton); return ms_Singleton;}
};