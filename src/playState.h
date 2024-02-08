#pragma once

#include <Ogre.h>
#include <OgreNewt.h>

#include "GameState.h"
#include "RigidBody.h"
#include "Paddle.h"
#include "Puck.h"
#include "Table.h"
#include "compai.h"

using namespace Ogre;
using namespace OgreNewt;

//easy medium hard
#define MAX_LEVELS 3

class PlayState : public GameState
{
public:

	enum States
	{
		STATE_COUNTDOWN,
		STATE_PLAYING,
		STATE_GOAL,
		STATE_GAME_OVER,
		STATE_WAIT,
		STATE_EMPTY,
	} ;



	void enter();
	void exit();

	void pause();
	void resume();

	//implement virtual functions from superclass
	void keyClicked(KeyEvent* e);
	void keyPressed(KeyEvent* e);
	void keyReleased(KeyEvent* e);

	void mouseMoved(MouseEvent* e);
	void mouseDragged(MouseEvent* e);
	void mousePressed(MouseEvent* e) {};
	void mouseReleased(MouseEvent* e) {};

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);

	static PlayState* getInstance() { return &mPlayState; }


	bool initOSMScene(String sceneFilename);
	void initRigidBodies();
	void initPhysicsWorld();
	void initOverlays();
	void initAI();
	void initLevelVars();
	void initLevel(int n);

	void nextLevel();


	//update frames per second stats
	void updateFPS();

	void resetPositions();

	void updateState();
	void updateStateCountdown();
	void updateStatePlaying();
	void updateStateGoal();
	void updateStateGameOver();
	void updateStateWait();

	void checkPuckGoal();


	void setGoalPlayer();
	void setGoalComp();
	void setGameOver();

	void stopPuckAndPaddle();


protected:
	PlayState();

	//pointers to important objects
	OverlayManager* mOverlayMgr;

	World *mPhysicsWorld;
	BasicFrameListener *mPhysicsListener;

	CompAI mAI;

	bool mIsExitGame;
	bool mIsPlayerTurn;

	//paddles
	Paddle *mPaddlePlayer;
	Paddle *mPaddleComp;

	//puck
    Puck   *mPuck;

	//table parts
	Table  *mTableWall;
	Table  *mTableSurface;

	Real mTimeSinceLastFrame;

	Real mTimeCountDown;
	Real mTimeGoal;
	Real mTimeGameOver;
	Real mTimeWait;

	States mCurrentState;

	int mLivesLeftPlayer;
	int mLivesLeftComp;
	int mCurrentLevel;

	bool mIsShowFPS;
	
private:
	static PlayState mPlayState;
};

