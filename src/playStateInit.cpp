#include <Ogre.h>
#include "PlayState.h"
#include "physicsMat.h"
#include "..\\globals.h"
#include "..\\include\\OgreOSMScene.h"

using namespace Ogre;

PlayState::PlayState()
{
	mCurrentState = STATE_EMPTY;
	mIsExitGame = false;
	mTimeSinceLastFrame = 0;
	mCurrentLevel = 0;
	mIsShowFPS = false;
	
	mRoot = NULL;
	mSceneMgr = NULL;
	mOverlayMgr = NULL;
	mViewport = NULL;
	mCamera = NULL;
	mPhysicsWorld = NULL;
	mPhysicsListener = NULL;
	mPaddlePlayer = NULL;
	mPaddleComp = NULL;
    mPuck = NULL;
	mTableWall = NULL;
	mTableSurface = NULL;
}


//load ofusion scene. returns true when successful, false if not
bool PlayState::initOSMScene(String sceneFilename)
{
	OSMScene *osm = new OSMScene(mSceneMgr, mRoot->getAutoCreatedWindow());
	osm->initialise(sceneFilename.c_str() );
	osm->createScene();

	// Get the list of cameras loaded with the scene
	OSMScene::CameraList camList = osm->getCameraList();
	
	if(!camList.empty())
		mCamera = camList[0];

	delete osm;

	return true;
}

//init all physics objects.
void PlayState::initRigidBodies()
{
	mPaddlePlayer = new Paddle;
	mPaddlePlayer->init(mSceneMgr, "paddlePlayer", mPhysicsWorld, PADDLE_RADIUS, PADDLE_HEIGHT);
	mPaddlePlayer->setBoundsZ(0, TABLE_DIM_Z_HALF);
	mPaddlePlayer->setPlayerControlled(true);

	mPaddleComp = new Paddle;
	mPaddleComp->init(mSceneMgr, "paddleComp", mPhysicsWorld, PADDLE_RADIUS, PADDLE_HEIGHT);
   	mPaddleComp->setBoundsZ(-TABLE_DIM_Z_HALF, 0);
	mPaddleComp->setPlayerControlled(false);

	mPuck = new Puck;
	mPuck->init(mSceneMgr, "puck", mPhysicsWorld, PUCK_RADIUS, PUCK_HEIGHT);

	mTableWall = new Table;
	mTableWall->init(mSceneMgr, "tableWall", mPhysicsWorld, 
		             PhysicsMat::getSingletonPtr()->getTableWallMatID());

	mTableSurface = new Table;
	mTableSurface->init(mSceneMgr, "tableSurface", mPhysicsWorld, 
		             PhysicsMat::getSingletonPtr()->getTableSurfaceMatID());

	resetPositions();
}

//init newton world and physics materials
void PlayState::initPhysicsWorld()
{
	mPhysicsWorld = new World;
	mPhysicsListener = new BasicFrameListener(mRoot->getAutoCreatedWindow(),
											  mSceneMgr,
											  mPhysicsWorld,
											  PHYSICS_UPDATE_FPS);

	mPhysicsWorld->setWorldSize(Vector3 (-100, -100, -100), Vector3(100,100,100));
	
	new PhysicsMat;
	PhysicsMat::getSingletonPtr()->init(mPhysicsWorld);
}

//init overlays
void PlayState::initOverlays()
{
	mOverlayMgr = OverlayManager::getSingletonPtr();
	mOverlayMgr->getByName("SH/PlayState/Overlay")->show();

	if (mIsShowFPS)
		mOverlayMgr->getByName("SH/Stats/Overlay")->show();
	else
		mOverlayMgr->getByName("SH/Stats/Overlay")->hide();

	
	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->show();

	mOverlayMgr->getOverlayElement("SH/PlayState/PlayerLife1")->show();
	mOverlayMgr->getOverlayElement("SH/PlayState/PlayerLife2")->show();
	mOverlayMgr->getOverlayElement("SH/PlayState/PlayerLife3")->show();

	mOverlayMgr->getOverlayElement("SH/PlayState/CompLife1")->show();
	mOverlayMgr->getOverlayElement("SH/PlayState/CompLife2")->show();
	mOverlayMgr->getOverlayElement("SH/PlayState/CompLife3")->show();
}

//init computer ai
void PlayState::initAI()
{
	mAI.init(mPaddleComp, mPuck, -TABLE_DIM_Z_HALF, TABLE_DIM_Z_HALF);
}

//init level variables
void PlayState::initLevelVars()
{
	mLivesLeftPlayer = 3;
	mLivesLeftComp = 3;
	mIsPlayerTurn = true;
}

//init ai difficulty
void PlayState::initLevel(int n)
{
	initLevelVars();

	switch (n)
	{
	case 0:
		mAI.setDefense(20);
		mAI.setOffense(20);
		mAI.setSpeedMult(7.0);
		break;

	case 1:
		mAI.setDefense(50);
		mAI.setOffense(40);
		mAI.setSpeedMult(8.0);
		break;

	case 2:
		mAI.setDefense(80);
		mAI.setOffense(70);
		mAI.setSpeedMult(10.0);
		break;

	default:
		mAI.setDefense(80);
		mAI.setOffense(70);
		mAI.setSpeedMult(10.0);
		break;

	}
}

//reset puck and paddle positions and stop them from moving
void PlayState::resetPositions()
{
	mPaddlePlayer->setPosition(Vector3(0,0.1,30));
	mPaddlePlayer->stopBody();
	mPaddlePlayer->move(Vector3(0,0,0));

	mPaddleComp->setPosition(Vector3(0,0.1,-30));
	mPaddleComp->stopBody();

	if (mIsPlayerTurn)
		mPuck->setPosition(Vector3(0,0.1,20));
	else
		mPuck->setPosition(Vector3(0,0.1,-20));

	mPuck->stopBody();
}

