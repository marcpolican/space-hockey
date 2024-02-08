#pragma once

//reference:
//http://www.ogre3d.org/wiki/index.php/Managing_Game_States_with_OGRE

#include <vector>
#include <Ogre.h>
#include <OgreEventListeners.h>
#include <OgreSingleton.h>

#include "InputManager.h"
#include "SoundManager.h"

using namespace Ogre;

class GameState;

class GameManager : public FrameListener, public KeyListener, public MouseMotionListener, public MouseListener,
	public Singleton<GameManager>
{

public:

	GameManager();
	~GameManager();

	void start(GameState* state);
	void changeState(GameState* state);
	void pushState(GameState* state);
	void popState();
	static GameManager& getSingleton(void);
	static GameManager* getSingletonPtr(void);


protected:

	Root* mRoot;
	RenderWindow* mRenderWindow;
	InputManager* mInputManager;
	SoundManager* mSoundManager;

	void setupResources(void);
	bool configure(void);

	void keyClicked(KeyEvent* e);
	void keyPressed(KeyEvent* e);
	void keyReleased(KeyEvent* e);

	void mouseMoved(MouseEvent* e);
	void mouseDragged(MouseEvent* e);

	void mouseEntered(MouseEvent* e) {} 
	void mouseExited(MouseEvent* e) {}
	void mouseClicked(MouseEvent* e) {}
	void mousePressed(MouseEvent* e);
	void mouseReleased(MouseEvent* e);
	
	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);


private:
	std::vector<GameState*> mStates;
};

