#pragma once

//reference:
//http://www.ogre3d.org/wiki/index.php/Managing_Game_States_with_OGRE

#include <OgreSingleton.h>
#include <OgreInput.h>

using namespace Ogre;

class InputManager : public Singleton<InputManager>
{
public:
	InputManager(RenderWindow* rwindow);
	virtual ~InputManager();

	InputReader* getInputDevice() const { return mInputDevice; }
	EventProcessor* getEventProcessor() const { return mEventProcessor; }

	static InputManager& getSingleton(void);
	static InputManager* getSingletonPtr(void);

private:
	EventProcessor* mEventProcessor;
	InputReader* mInputDevice;
};

