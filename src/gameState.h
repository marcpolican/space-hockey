#pragma once


//reference:
//http://www.ogre3d.org/wiki/index.php/Managing_Game_States_with_OGRE

#include <ogre.h>

#include "GameManager.h"
class GameState
{
public:
	//called by game manager when state is started
	virtual void enter() = 0;

	virtual void exit() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	//key events
	virtual void keyClicked(Ogre::KeyEvent* e) = 0;
	virtual void keyPressed(Ogre::KeyEvent* e) = 0;
	virtual void keyReleased(Ogre::KeyEvent* e) = 0;

	//mouse events
	virtual void mouseMoved(Ogre::MouseEvent* e) = 0;
	virtual void mouseDragged(Ogre::MouseEvent* e) = 0;
	virtual void mousePressed(MouseEvent* e) = 0;
	virtual void mouseReleased(MouseEvent* e) = 0;

	//called everytime a frame will be rendered
	virtual bool frameStarted(const Ogre::FrameEvent& evt) = 0;

	//called after rendering a frame
	virtual bool frameEnded(const Ogre::FrameEvent& evt) = 0;

	void changeState(GameState* state) { GameManager::getSingletonPtr()->changeState(state); }
	void pushState(GameState* state) { GameManager::getSingletonPtr()->pushState(state); }
	void popState() { GameManager::getSingletonPtr()->popState(); }

protected:
	GameState() { }


	Root *mRoot;
	SceneManager* mSceneMgr;
	Viewport* mViewport;
	Camera* mCamera;
};

