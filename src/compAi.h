#pragma once
#include "puck.h"
#include "paddle.h"

using namespace Ogre;
using namespace OgreNewt;

class PlayState;

class CompAI
{
	typedef enum
	{
		STATE_NONE,
		STATE_PUCK_PLAYER_SIDE,
		STATE_PUCK_COMP_SIDE,
		STATE_WAIT,
	} AIState;


	Paddle* mPaddle;
	Puck* mPuck;
	AIState mLastAIState;
	AIState mCurrAIState;
	bool mIsStateChanged;
	bool mIsDefend;
	bool mIsForceGoal;

	Vector3 mPuckPos;
	Vector3 mPuckVel;
	Vector3 mPaddlePos;
	Real mSqrDistance;
	Real mGuardGoalZ;
	Real mTargetGoalZ;
	Real mTimeSinceLastFrame;

	int mDefenseVal;
	int mOffenseVal;

	Real mTimeWait;
	Real mSpeedMult;

public:
	CompAI(void);
	~CompAI(void);

	void init (Paddle *paddle, Puck* puck, Real guardGoalZ, Real TargetGoalZ);

	void update(Real timeSinceLastFrame);
	void reset();

	bool isDefend();
	void defend();

	bool isForceGoal();
	void forceGoal();

	bool isStateChanged();
	bool isDangerGoingToGoal();
	void random();

	void wait();

	bool isInStrikingDistance();
	void returnToBase();

	void movePaddleToPuck();
	void movePaddleToPuckHit();
	void movePaddleToMiddle();

	void defenseDec(int n);
	void defenseInc(int n);
	void OffenseDec(int n);
	void offenseInc(int n);

	int getOffense() {return mOffenseVal;}
	int getDefense() {return mDefenseVal;}

	void setOffense(int n) {mOffenseVal = n;}
	void setDefense(int n) {mDefenseVal = n;}

	Real geSpeedMult() {return mSpeedMult;}
	void  setSpeedMult(Real f) {mSpeedMult = f;}

	void SetGuardGoalZ(Real z) {mGuardGoalZ = z;}
	void SetTargetGoalZ(Real z) {mTargetGoalZ = z;}
};
