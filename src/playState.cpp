#include <Ogre.h>
#include <OgreKeyEvent.h>
#include "..\\globals.h"
#include "PlayState.h"
#include "MenuState.h"
#include "PhysicsMat.h"

#define TIME_COUNTDOWN 5
#define TIME_GOAL 0
#define TIME_GAME_OVER 3
#define TIME_WAIT 1


PlayState PlayState::mPlayState;

void PlayState::enter()
{
	mIsExitGame = false;
	mCurrentState = STATE_COUNTDOWN;
	mTimeCountDown = TIME_COUNTDOWN;
	mTimeGoal = TIME_GOAL;
	mTimeGameOver = TIME_GAME_OVER;
	mTimeWait = TIME_WAIT;

	mRoot = Root::getSingletonPtr();

	//create scene manager. camera and viewport for this state
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);

	//call init functions
	initLevel(mCurrentLevel);
	initPhysicsWorld();
	initOSMScene("scene.osm");
	initRigidBodies();
	initOverlays();
	initAI();
	
	SoundManager::getSingletonPtr()->startMusic();
}

void PlayState::exit()
{
	//clean up
	SoundManager::getSingletonPtr()->stopMusic();

	Debugger::getSingleton().deInit();

	//hide overlays
	mOverlayMgr->getByName("SH/Stats/Overlay")->hide();
	mOverlayMgr->getByName("SH/PlayState/Overlay")->hide();

	//clear scene manager
	mSceneMgr->clearScene();
	mSceneMgr->destroyAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();

	if (mPuck) delete mPuck;
	if (mPaddleComp) delete mPaddleComp;
	if (mPaddlePlayer) delete mPaddlePlayer;
	if (mTableWall) delete mTableWall;
	if (mTableSurface) delete mTableSurface;

	if (mPhysicsListener) delete mPhysicsListener;
	if (mPhysicsWorld) delete mPhysicsWorld;

	if (PhysicsMat::getSingletonPtr()) 
		delete PhysicsMat::getSingletonPtr();
}

void PlayState::keyReleased(KeyEvent* e)
{
	//go back to main menu
	if (e->getKey() == KC_ESCAPE)
		mIsExitGame = true;

	//show hide FPS
	if (e->getKey() == KC_F)
	{
		mIsShowFPS = ! mIsShowFPS;
		if (mIsShowFPS)
			mOverlayMgr->getByName("SH/Stats/Overlay")->show();
		else
			mOverlayMgr->getByName("SH/Stats/Overlay")->hide();

	}
	
}

void PlayState::pause() {}
void PlayState::resume() {}
void PlayState::keyClicked(KeyEvent* e) {}
void PlayState::keyPressed(KeyEvent* e) {}
void PlayState::mouseMoved(MouseEvent* e) {}
void PlayState::mouseDragged(MouseEvent* e) {}


bool PlayState::frameStarted(const FrameEvent& evt)
{
	InputReader *inputReader = InputManager::getSingletonPtr()->getInputDevice();
	inputReader->capture();


	mTimeSinceLastFrame = evt.timeSinceLastFrame;
	
	//handle mouse input
	if (mCurrentState == STATE_PLAYING)
	{
		//move paddle
		long lx = inputReader->getMouseRelativeX();
		long ly = inputReader->getMouseRelativeY();

		if (lx == 0 && ly == 0)
		{
			mPaddlePlayer->move(Vector3(0,0,0));
		}
		else
		{
			Real x = (Real) lx / (Real) mRoot->getAutoCreatedWindow()->getWidth();
			Real y = (Real) ly / (Real) mRoot->getAutoCreatedWindow()->getHeight();
			x *= (MOUSE_MOVEMENT_MULT);
			y *= (MOUSE_MOVEMENT_MULT);
			mPaddlePlayer->move(Vector3(x, 0, y));
		}
	}

	SceneNode *camNode = mSceneMgr->getSceneNode("userCam");
	Vector3 pos;
	if (inputReader->isKeyDown(KC_UP))
	{
		pos = camNode->getPosition();
		pos.y += 20.0 * mTimeSinceLastFrame;
		if (pos.y >= 100)
			pos.y = 100;

		camNode->setPosition(pos);
	}
	else if (inputReader->isKeyDown(KC_DOWN))
	{
		pos = camNode->getPosition();
		pos.y -= 20.0 * mTimeSinceLastFrame;
		if (pos.y <= 10)
			pos.y = 10;

		camNode->setPosition(pos);
	}


	updateState();

	//update physics
	if (!mPhysicsListener->frameStarted(evt))
		return false;

	updateFPS();

	if (mIsExitGame)
		GameManager::getSingletonPtr()->changeState(MenuState::getInstance());

	return true;
}

bool PlayState::frameEnded(const FrameEvent& evt)
{
	if (!mPhysicsListener->frameEnded(evt))
		return false;

	return true;
}

void PlayState::updateFPS()
{
	if (mIsShowFPS)
	{
		static String currFps = "Current FPS: ";
		static String bestFps = "Best FPS: ";
		static String worstFps = "Worst FPS: ";
		static String tris = "Tri Count: ";

		// update stats when necessary
		OverlayElement* guiCurr = mOverlayMgr->getOverlayElement("SH/Stats/FPS");
		OverlayElement* guiBest = mOverlayMgr->getOverlayElement("SH/Stats/FPSWorst");
		OverlayElement* guiWorst = mOverlayMgr->getOverlayElement("SH/Stats/FPSBest");

		const RenderTarget::FrameStats& stats = mRoot->getAutoCreatedWindow()->getStatistics();

		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = mOverlayMgr->getOverlayElement("SH/Stats/Tris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));
	}
}

void PlayState::updateState()
{
	switch (mCurrentState)
	{
	case STATE_EMPTY:
		mCurrentState = STATE_COUNTDOWN;
		
	case STATE_COUNTDOWN:
		updateStateCountdown(); break;

	case STATE_GOAL:
		updateStateGoal(); break;

	case STATE_PLAYING:
		updateStatePlaying(); break;

	case STATE_GAME_OVER:
		updateStateGameOver(); break;

	case STATE_WAIT:
		updateStateWait(); break;

	default:
		mCurrentState = STATE_COUNTDOWN;
	}
}

//STATE_WAIT
void PlayState::updateStateWait()
{
	mTimeWait -= mTimeSinceLastFrame;
	if (mTimeWait < 0)
	{
		mCurrentState = STATE_GOAL;
		mTimeWait = TIME_WAIT;
		resetPositions();
	}
}

//STATE_COUNTDOWN
void PlayState::updateStateCountdown()
{
	mTimeCountDown -= mTimeSinceLastFrame;
	if (mTimeCountDown >= 0)
	{
		Vector3 pos;
		OverlayElement *countdown =  mOverlayMgr->getOverlayElement("SH/PlayState/Countdown");

		if (mTimeCountDown <= 1)
			countdown->setCaption( "start" );
		else if (mTimeCountDown > 4)
			countdown->setCaption( "Easy round" );
		else
			countdown->setCaption( StringConverter::toString( (int)mTimeCountDown ) );

		//use this time to set the clampy for the puck and paddles
		if (mPuck->isBodySleeping())
		{
			pos = mPuck->getPosition();
			mPuck->setClampY(pos.y);
			mPuck->mBody->setAutoFreeze(0);
			mPuck->mBody->unFreeze();
		}

		if (mPaddleComp->isBodySleeping())
		{
			pos = mPaddleComp->getPosition();
			mPaddleComp->setClampY(pos.y);
			mPaddleComp->mBody->setAutoFreeze(0);
			mPaddleComp->mBody->unFreeze();
		}

		if (mPaddlePlayer->isBodySleeping())
		{
			pos = mPaddlePlayer->getPosition();
			mPaddlePlayer->setClampY(pos.y);
			mPaddlePlayer->mBody->setAutoFreeze(0);
			mPaddlePlayer->mBody->unFreeze();
		}
	}
	else
	{
		mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->hide();
		mCurrentState = STATE_PLAYING;
		mTimeCountDown = TIME_COUNTDOWN;
	}	

}

//STATE_PLAYING
void PlayState::updateStatePlaying()
{
	mAI.update(mTimeSinceLastFrame);

	if (mPuck->getIsCollideToPaddle())
	{
		SoundManager::getSingletonPtr()->playHit();
		mPuck->setIsCollideToPaddle(false);
	}

	//check if puck goes into goal
	checkPuckGoal();
}

//STATE_GOAL
void PlayState::updateStateGoal()
{
	mTimeGoal -= mTimeSinceLastFrame;
	if (mTimeGoal <= 0)
	{
		mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->hide();
		mCurrentState = STATE_PLAYING;
		mTimeGoal = TIME_GOAL;
	}
}

//STATE_GAME_OVER
void PlayState::updateStateGameOver()
{
	mTimeGameOver -= mTimeSinceLastFrame;
	if (mTimeGameOver <= 0)
	{
		mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->hide();
		mTimeGameOver = TIME_GAME_OVER;

		mIsExitGame = true;
	}
}

//check if puck went into one of the two goals
void PlayState::checkPuckGoal()
{
	Vector3 pos = mPuck->getPosition();
	if (pos.z < -TABLE_DIM_Z_HALF - PUCK_RADIUS)
	{
		if (-GOAL_WIDTH/2  <  pos.x && pos.x  <  GOAL_WIDTH/2)
			setGoalPlayer();
	}
	else if (pos.z > TABLE_DIM_Z_HALF + PUCK_RADIUS)
	{
		if (-GOAL_WIDTH/2  <  pos.x && pos.x  <  GOAL_WIDTH/2)
			setGoalComp();
			
	}
}

//score for the player
void PlayState::setGoalPlayer()
{
	SoundManager::getSingletonPtr()->playGoal();

	//remove a comp life
	char szTemp[32] = "";
	sprintf (szTemp, "SH/PlayState/CompLife%d", mLivesLeftComp);
	mOverlayMgr->getOverlayElement(szTemp)->hide();
	mLivesLeftComp --;

	if (mLivesLeftComp == 0)
	{
		if (mCurrentLevel != MAX_LEVELS - 1)
		{
			nextLevel();

			if (mCurrentLevel == 1)
                mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->setCaption("Medium round");
			else if (mCurrentLevel == 2)
				mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->setCaption("Hard round");

			mTimeWait = TIME_WAIT+1;

			mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->show();
			mCurrentState = STATE_WAIT;

			stopPuckAndPaddle();
		}
		else
		{
			setGameOver();
		}
		return;
	}

	mIsPlayerTurn = false;

	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->setCaption("Player scores!!");
	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->show();
	mCurrentState = STATE_WAIT;

	stopPuckAndPaddle();
}


//score for the computer
void PlayState::setGoalComp()
{
	SoundManager::getSingletonPtr()->playGoal();

	//remove a comp life
	char szTemp[32] = "";
	sprintf (szTemp, "SH/PlayState/PlayerLife%d", mLivesLeftPlayer);
	mOverlayMgr->getOverlayElement(szTemp)->hide();
	mLivesLeftPlayer --;

	if (mLivesLeftPlayer == 0)
	{
		setGameOver();
		return;
	}

	mIsPlayerTurn = true;

	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->setCaption("Computer scores!!");
	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->show();
	mCurrentState = STATE_WAIT;

	stopPuckAndPaddle();
}


//game over
void PlayState::setGameOver()
{
	char szTemp[64];

	if (mLivesLeftComp == 0)
	{
		if (mCurrentLevel == MAX_LEVELS - 1)
			sprintf (szTemp, "Player wins!\nThank you for playing");
		mIsPlayerTurn = false;
	}
	else
	{
		sprintf (szTemp, "Game over\nPlayer lost");
		mIsPlayerTurn = true;
	}

	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->setCaption(szTemp);
	mOverlayMgr->getOverlayElement("SH/PlayState/Countdown")->show();
	mCurrentState = STATE_GAME_OVER;

	resetPositions();
}

//go to next level
void PlayState::nextLevel()
{
	mCurrentLevel++;
	initLevel(mCurrentLevel);
	initOverlays();
}

void PlayState::stopPuckAndPaddle()
{
	mPuck->stopBody();
	mPaddleComp->stopBody();
	mPaddlePlayer->stopBody();
	mPaddlePlayer->move(Vector3(0,0,0));
}