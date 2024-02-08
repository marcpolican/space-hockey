#pragma once

#include <ogre.h>
#include "Gamestate.h"
#include "overlayButton.h"

using namespace Ogre;

class MenuState : public GameState
{
public:

	enum Buttons
	{
		BTN_PLAY,
		BTN_EXIT,
		BTN_MAX,
	};

	//called by game manager when state is started
	virtual void enter();

	virtual void exit();
	virtual void pause() {}
	virtual void resume() {}

	//key events
	virtual void keyClicked(KeyEvent* e) {}
	virtual void keyPressed(KeyEvent* e) {}
	virtual void keyReleased(KeyEvent* e);

	//mouse events
	virtual void mouseMoved(MouseEvent* e);
	virtual void mouseDragged(MouseEvent* e);
	virtual void mousePressed(MouseEvent* e);
	virtual void mouseReleased(MouseEvent* e);

	//called everytime a frame will be rendered
	virtual bool frameStarted(const FrameEvent& evt);

	//called after rendering a frame
	virtual bool frameEnded(const FrameEvent& evt) {return true;}

	static MenuState* getInstance() { return &mMenuState; }

protected:
	MenuState() { }


	OverlayButton mButtons[BTN_MAX];
	bool mIsExitGame;
	bool mIsStartGame;

	OverlayElement *mCursor;


private:
	static MenuState mMenuState;
};

