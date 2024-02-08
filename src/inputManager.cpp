#include <ogre.h>
#include <OgreEventProcessor.h>
#include "InputManager.h"
#include "globals.h"

template<> InputManager* Singleton<InputManager>::ms_Singleton = 0;

InputManager::InputManager(RenderWindow* rwindow)
{
	//initialize event processor
	mEventProcessor = new EventProcessor();
	mEventProcessor->initialise(rwindow);
	mEventProcessor->startProcessingEvents();

	//initialize input device
	mInputDevice = PlatformManager::getSingleton().createInputReader();
	mInputDevice->initialise(rwindow,true, true);
}

InputManager::~InputManager()
{
	if (mEventProcessor) delete mEventProcessor;
	PlatformManager::getSingletonPtr()->destroyInputReader(mInputDevice);
}

InputManager* InputManager::getSingletonPtr(void)
{
	return ms_Singleton;
}

InputManager& InputManager::getSingleton(void)
{
	assert(ms_Singleton);
	return *ms_Singleton;
}