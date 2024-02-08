#include <OgreKeyEvent.h>
#include "menustate.h"
#include "playstate.h"
#include "soundmanager.h"

using namespace Ogre;


MenuState MenuState::mMenuState;

void MenuState::enter()
{
	mRoot = Root::getSingletonPtr();

	//create scene manager. camera and viewport for this state
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);
	mCamera = mSceneMgr->createCamera("IntroCamera");
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	
	mIsExitGame = false;

	//init menu screen overlay
	OverlayManager::getSingletonPtr()->getByName("SH/MenuState/Overlay")->show();
	mButtons[BTN_PLAY].loadButton("SH/MenuState/BtnPlay");
	mButtons[BTN_EXIT].loadButton("SH/MenuState/BtnExit");

	//init cursor overlay
	OverlayManager::getSingletonPtr()->getByName("SH/Cursor/Overlay")->show();
	mCursor = OverlayManager::getSingletonPtr()->getOverlayElement("SH/Cursor/Cursor");
	mCursor->setPosition(0.5, 0.5);
}

void MenuState::exit()
{
	//hide all overlays
	OverlayManager::getSingletonPtr()->getByName("SH/MenuState/Overlay")->hide();
	OverlayManager::getSingletonPtr()->getByName("SH/Cursor/Overlay")->hide();

	//clear the scene manager
	mSceneMgr->clearScene();
	mSceneMgr->destroyAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void MenuState::mousePressed(MouseEvent* e)
{
	if (!mIsStartGame)
	{
		//send mouse pressed events to the buttons
		for (int i=0; i<BTN_MAX; i++)
			mButtons[i].mousePressed(e->getX(), e->getY());
	}
}

void MenuState::mouseReleased(MouseEvent* e)
{
	if (mIsStartGame)
	{
		mIsStartGame = false;
		GameManager::getSingletonPtr()->changeState(PlayState::getInstance());
		OverlayManager::getSingletonPtr()->getByName("SH/HelpScreen/Overlay")->hide();
	}
	else
	{
		//send mouse released events to the buttons
		for (int i=0; i<BTN_MAX; i++)
			mButtons[i].mouseReleased(e->getX(), e->getY());
	}
}

void MenuState::mouseMoved(MouseEvent* e)
{
	//move cursor position
	mCursor->setPosition(e->getX(), e->getY());
}

void MenuState::mouseDragged(MouseEvent* e)
{
	//move cursor position
	mCursor->setPosition(e->getX(), e->getY());
}


void MenuState::keyReleased(KeyEvent* e)
{
	//handle key release
	if (e->getKey() == KC_ESCAPE)
		mIsExitGame = true;
}

bool MenuState::frameStarted(const FrameEvent& evt)
{
	if (mIsExitGame)
		return false; 

	if (mButtons[BTN_PLAY].isClicked())
	{
		//start game
		mIsStartGame = true;
		OverlayManager::getSingletonPtr()->getByName("SH/HelpScreen/Overlay")->show();
		SoundManager::getSingletonPtr()->playClick();
		return true;
	}

	if (mButtons[BTN_EXIT].isClicked())
	{
		//exit game
		SoundManager::getSingletonPtr()->playClick();
		return false;
	}

	return true;
}
