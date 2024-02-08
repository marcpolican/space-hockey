#pragma once

#include <ogre.h>

using namespace Ogre;

class OverlayButton
{
public:

	enum ButtonStates
	{
		BUTTON_UP,	BUTTON_DOWN, BUTTON_DISABLED,
	};

	OverlayButton(void);
	~OverlayButton(void);

	void loadButton(const String &name);
	void mousePressed(Real x, Real y);
	void mouseReleased(Real x, Real y);

	void setEnabled(bool bIsEnabled);
	bool isDown() {return mState == BUTTON_DOWN;}
	bool isClicked(bool resetAfterUse = true);

	void show();
	void hide();

	virtual void down();
	virtual void up();
	virtual void clicked();

protected:
	bool mWasMouseDown;
	bool mIsClicked;
	bool mIsEnabled;
	ButtonStates mState;
	OverlayElement *mSpriteUp;
	OverlayElement *mSpriteDown;

	//overlay bounds
	Real mTop, mLeft, mBottom, mRight;
};
