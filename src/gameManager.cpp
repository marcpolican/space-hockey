#include <Ogre.h>
#include "GameManager.h"
#include "InputManager.h"
#include "GameState.h"
#include "globals.h"
#include "windows.h"

using namespace Ogre;

template<> GameManager* Singleton<GameManager>::ms_Singleton = 0;

//constructor
GameManager::GameManager()
{
	mRoot = 0;
	mInputManager = 0;
	mSoundManager = 0;
}

//destructor
GameManager::~GameManager()
{
	// clean up all the states
	while (!mStates.empty()) {
		mStates.back()->exit();
		mStates.pop_back();
	}

	if (mInputManager) delete mInputManager;
	if (mSoundManager) delete mSoundManager;
	if (mRoot) delete mRoot;
}

void GameManager::start(GameState* state)
{
	mRoot = new Root();

	setupResources();

	if (!configure()) 
		return;

	mRoot->addFrameListener(this);

	mInputManager = new InputManager(mRoot->getAutoCreatedWindow());
	mInputManager->getEventProcessor()->addKeyListener(this);
	mInputManager->getEventProcessor()->addMouseMotionListener(this);
	mInputManager->getEventProcessor()->addMouseListener(this);

	mSoundManager = new SoundManager;
	mSoundManager->initAllSounds();

	OverlayManager::getSingletonPtr()->getByName("SH/MenuState/Overlay")->show();

	changeState(state);

	mRoot->startRendering();
}

void GameManager::changeState(GameState* state)
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mStates.back()->exit();
		mStates.pop_back();
	}

	// store and init the new state
	mStates.push_back(state);
	mStates.back()->enter();
}

void GameManager::pushState(GameState* state)
{
	// pause current state
	if ( !mStates.empty() ) {
		mStates.back()->pause();
	}

	// store and init the new state
	mStates.push_back(state);
	mStates.back()->enter();
}

void GameManager::popState()
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mStates.back()->exit();
		mStates.pop_back();
	}

	// resume previous state
	if ( !mStates.empty() ) {
		mStates.back()->resume();
	}
}

void GameManager::setupResources(void)
{
	// load resource paths from config file
	ConfigFile cf;
	cf.load("resources.cfg");

	// go through all settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin() ; i != settings->end() ; ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

bool GameManager::configure(void)
{
	// load config settings from ogre.cfg
	if (!mRoot->restoreConfig())
	{
		// if there is no config file, show the configuration dialog
		if (!mRoot->showConfigDialog())
		{
			return false;
		}
	}
	
	mRoot->getRenderSystem()->setConfigOption("VSync", "Yes");

	// initialise and create a default rendering window
	mRenderWindow = mRoot->initialise(true, "Space Hockey");

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
}


void GameManager::keyClicked(KeyEvent* e)
{
	// call keyClicked of current state
	mStates.back()->keyClicked(e);
}

void GameManager::keyPressed(KeyEvent* e)
{
	// call keyPressed of current state
	mStates.back()->keyPressed(e);
}

void GameManager::keyReleased(KeyEvent* e)
{
	// call keyReleased of current state
	mStates.back()->keyReleased(e);
}


void GameManager::mouseMoved(MouseEvent* e)
{
	//call mousemoved of current state
	mStates.back()->mouseMoved(e);
}

void GameManager::mouseDragged(MouseEvent* e)
{
	//call mousedragged of current state
	mStates.back()->mouseDragged(e);
}

void GameManager::mousePressed(MouseEvent* e) 
{
	//call mousePressed of current state
	mStates.back()->mousePressed(e);
}

void GameManager::mouseReleased(MouseEvent* e) 
{
	//call mouseReleased of current state
	mStates.back()->mouseReleased(e);
}


bool GameManager::frameStarted(const FrameEvent& evt)
{
	if (!mRenderWindow->isActive()) 
		return true;

	// call frameStarted of current state
	return mStates.back()->frameStarted(evt);
}

bool GameManager::frameEnded(const FrameEvent& evt)
{
	// call frameEnded of current state
	mStates.back()->frameEnded(evt);

	/*
	//limit to 60 fps
	Real wait = (1000.0 / 50) - (1000.0 * evt.timeSinceLastFrame);
	if (wait > 0)
	{
		DWORD dwWait = (DWORD) wait;
		Sleep(dwWait); 
	}
	*/
	

   return true;
}

GameManager* GameManager::getSingletonPtr(void)
{
	return ms_Singleton;
}

GameManager& GameManager::getSingleton(void)
{  
	assert(ms_Singleton);
	return *ms_Singleton;
}

