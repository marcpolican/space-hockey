#include "playstate.h"
#include "globals.h"
#include "compai.h"

#define IS_GUARD_GOAL_NEAR_SIDE (mGuardGoalZ > 0.0f)
#define MAX_RAND 100

#define DEFAULT_AI_DEFENSE 25
#define DEFAULT_AI_OFFENSE 25


CompAI::CompAI(void)
{
	mPaddle = NULL;
	mPuck = NULL;
	
	mIsStateChanged = false;
	mIsDefend = false;
	mIsForceGoal = false;
	mSqrDistance = 0;

	mCurrAIState = STATE_NONE;
	mLastAIState = STATE_NONE;
	mDefenseVal = DEFAULT_AI_DEFENSE;
	mOffenseVal = DEFAULT_AI_OFFENSE;
	mSpeedMult = 2.0f;
	mGuardGoalZ = 0.0f;
	mTargetGoalZ = 0.0f;

	mTimeWait = 0;
}

CompAI::~CompAI(void)
{
}

//initialization
void CompAI::init (Paddle *paddle, Puck* puck, Real guardGoalZ, Real targetGoalZ)
{
	mPaddle = paddle;
	mPuck = puck;
	mTargetGoalZ = targetGoalZ;
	mGuardGoalZ = guardGoalZ;
}

void CompAI::reset()
{
	mIsStateChanged = false;
	mIsDefend = false;
	mIsForceGoal = false;
	mCurrAIState = STATE_NONE;
	mLastAIState = STATE_NONE;
}

void CompAI::update(Real timeSinceLastFrame)
{
	mTimeSinceLastFrame = timeSinceLastFrame;

	//save positions and velocity
	mPuckPos = mPuck->getPosition();
	mPuckVel = mPuck->mBody->getVelocity();
	mPaddlePos = mPaddle->getPosition();

	//error checking dont do anything if the puck is outside the table
	if (mPuckPos.x < -TABLE_DIM_X_HALF || mPuckPos.x > TABLE_DIM_Z_HALF ||
		mPuckPos.z < -TABLE_DIM_Z_HALF || mPuckPos.z > TABLE_DIM_Z_HALF)
	{
		return;
	}

	//record distance from puck to paddle
	mSqrDistance = (mPuckPos.x - mPaddlePos.x) * (mPuckPos.x - mPaddlePos.x) + 
			       (mPuckPos.z - mPaddlePos.z) * (mPuckPos.z - mPaddlePos.z);

	//change state as necessary
	if (mCurrAIState != STATE_WAIT)
	{
		if ((mGuardGoalZ > 0 && mPuckPos.z < 0) ||	(mGuardGoalZ < 0 && mPuckPos.z > 0))
		{
			mCurrAIState = STATE_PUCK_PLAYER_SIDE;
		}
		else
		{
			//check if puck was hit.
			if (mPuck->getIsCollideToPaddle())
				mCurrAIState = STATE_WAIT;
			else
				mCurrAIState = STATE_PUCK_COMP_SIDE;
		}
	}

	if (mLastAIState != mCurrAIState)
	{
		mLastAIState = mCurrAIState;
		mIsStateChanged = true;
		
		mIsDefend = (rand() % MAX_RAND < mDefenseVal);
		mIsForceGoal = (rand() % MAX_RAND < mOffenseVal);
	}
	else
	{
		mIsStateChanged = false;
	}


	switch(mCurrAIState)
	{
	case STATE_NONE:
		break;

	case STATE_PUCK_COMP_SIDE:
		
		if (isDangerGoingToGoal() && isDefend())  //if puck is going towards the goal at high speed
			defend();
		
		else if (isForceGoal()) //force to shoot the puck
			forceGoal();
		
		else //just hit the puck
			random();

		break;
	case STATE_PUCK_PLAYER_SIDE:
		movePaddleToMiddle();
		break;

	case STATE_WAIT: //go the the center
		wait();
		break;
	}
}


void CompAI::wait()
{
	//dont do anything
	mTimeWait += mTimeSinceLastFrame;
	if (mTimeWait >= 0.3)
	{
		mTimeWait = 0;
		mCurrAIState = STATE_NONE;
	}
	else
	{
		if (IS_GUARD_GOAL_NEAR_SIDE)
			mPaddle->moveTo(0,0.0f,mGuardGoalZ - PADDLE_RADIUS,1.0f * mSpeedMult);
		else
			mPaddle->moveTo(0,0.0f,mGuardGoalZ + PADDLE_RADIUS,1.0f * mSpeedMult);
	}
}

bool CompAI::isDefend()
{
	return mIsDefend;
}
bool CompAI::isForceGoal()
{
	return mIsForceGoal;
}


void CompAI::defend()
{
	//defend the goal
	Real fZ;
	if (IS_GUARD_GOAL_NEAR_SIDE)
		fZ = mGuardGoalZ - PADDLE_RADIUS;
	else
		fZ = mGuardGoalZ + PADDLE_RADIUS;

	//place paddle just near the goal
	if (mPuckPos.x < -GOAL_WIDTH * 0.5f)
		mPaddle->moveTo(-GOAL_WIDTH * 0.5f,0.0f,fZ,5.0f * mSpeedMult);
	else if  (mPuckPos.x > GOAL_WIDTH * 0.5f)
		mPaddle->moveTo(GOAL_WIDTH * 0.5f,0.0f,fZ,5.0f * mSpeedMult);
	else
		mPaddle->moveTo(mPuckPos.x,0.0f,fZ,5.0f * mSpeedMult);
}

void CompAI::forceGoal()
{
	if (isInStrikingDistance())
	{
		//compute the direction where the puck should go to make a goal
		Real xStraight, xBankedLeft, xBankedRight;
		Real goalRange = (GOAL_WIDTH-PUCK_RADIUS + PUCK_RADIUS) * 0.5f;
		Real vel = ((rand() % 100) / 100.0f) + 0.5; //[0.5, 1.5]
		Real angle;

		vel *= 30;

		Vector3 paddleToPuck, puckToGoal;

		//add random values so that the puck does not always go in the middle
		xStraight = ((rand() % 100) - 50)/50.0f * goalRange;
		xBankedLeft = -TABLE_DIM_X + xStraight;
		xBankedRight = TABLE_DIM_X + xStraight;

		//get vectors
		paddleToPuck.x = mPaddlePos.x - mPuckPos.x;
		paddleToPuck.y = 0;
		paddleToPuck.z = mPaddlePos.z - mPuckPos.z;
		paddleToPuck.normalise();

		puckToGoal.x = mPuckPos.x - xStraight;
		puckToGoal.y = 0;
		puckToGoal.z = mPuckPos.z - mTargetGoalZ;
		puckToGoal.normalise();

		angle = puckToGoal.dotProduct(paddleToPuck);
		angle = TODEGREES(acos(angle));

		//strike now 
		if (angle > 90.0f) //it is impossible to shot a goal at this angle
		{
			movePaddleToPuckHit();
			mCurrAIState = STATE_WAIT;
		}
		else
		{
			//cheat puck movement
			if (angle >= 0.0f && angle <=15.0f) //STRAIGHT
				mPuck->moveTo(xStraight, 0.0f,mTargetGoalZ,vel * mSpeedMult); 
			else if (mPaddlePos.x < mPuckPos.x) //BANKED RIGHT
				mPuck->moveTo(xBankedRight, 0.0f, mTargetGoalZ, vel * mSpeedMult);
			else //BANKED LEFT
				mPuck->moveTo(xBankedLeft, 0.0f, mTargetGoalZ, vel * mSpeedMult);

			//set the flag to play hit sound
			mPuck->setIsCollideToPaddle(true);
		}
	}
	else
	{
		movePaddleToPuck();
	}
}

void CompAI::random()
{
	//just hit the puck
	if (isInStrikingDistance())
	{
		movePaddleToPuckHit();
		mCurrAIState = STATE_WAIT;
	}
	else
	{
		movePaddleToPuck();
	}
}

bool CompAI::isInStrikingDistance()
{
	if (mSqrDistance <= SQUARE(PUCK_RADIUS + PADDLE_RADIUS + 0.1))
		return true;

	return false;
}

bool CompAI::isDangerGoingToGoal()
{
	if ((IS_GUARD_GOAL_NEAR_SIDE && mPuckPos.z >= 0 && mPuckVel.z > 20.0f) ||
		(!IS_GUARD_GOAL_NEAR_SIDE && mPuckPos.z <= 0 && mPuckVel.z < -20.0f))
	{
		if (IS_GUARD_GOAL_NEAR_SIDE)
		{
			if (mPuckPos.z >= 10.0f)
				return true;
		}
		else
		{
			if (mPuckPos.z <= -10.0f)
				return true;
		}

		//get puck velocity vector magnitude
		if (mPuckVel.squaredLength() > 150 * 150)
			return true;
	}
	return false;
}

bool CompAI::isStateChanged()
{
	return mIsStateChanged;
}

//decrease defense value
void CompAI::defenseDec(int n)
{
	if (mDefenseVal - n < 0)
		mDefenseVal = 0;
	else
		mDefenseVal -= n;
}

//increase defense value
void CompAI::defenseInc(int n)
{
	if (mDefenseVal + n > MAX_RAND)
		mDefenseVal = 100;
	else
		mDefenseVal += n;
}

//decrease offense value
void CompAI::OffenseDec(int n)
{
	if (mOffenseVal - n < 0)
		mOffenseVal = 0;
	else
		mOffenseVal -= n;
}

//decrease offense value
void CompAI::offenseInc(int n)
{
	if (mOffenseVal + n > MAX_RAND)
		mOffenseVal = 100;
	else
		mOffenseVal += n;
}

void CompAI::movePaddleToPuck()
{
	if (IS_GUARD_GOAL_NEAR_SIDE)
		mPaddle->moveTo(mPuckPos.x,0.0f,mPuckPos.z+PUCK_RADIUS,5.0f * mSpeedMult);
	else
		mPaddle->moveTo(mPuckPos.x,0.0f,mPuckPos.z-PUCK_RADIUS,5.0f * mSpeedMult);
}

void CompAI::movePaddleToPuckHit()
{
	if (IS_GUARD_GOAL_NEAR_SIDE)
		mPaddle->moveTo(mPuckPos.x,0.0f,mPuckPos.z-3,5.0f * mSpeedMult);
	else
		mPaddle->moveTo(mPuckPos.x,0.0f,mPuckPos.z+3,5.0f * mSpeedMult);
}

void CompAI::movePaddleToMiddle()
{
	if (IS_GUARD_GOAL_NEAR_SIDE)
		mPaddle->moveTo(0.0,0.0f,mGuardGoalZ - 15.0f,1.0f * mSpeedMult);
	else
		mPaddle->moveTo(0.0,0.0f,mGuardGoalZ + 15.0f,1.0f * mSpeedMult);
}