/**********************************************************************
 *<
	oFusion Scene Loader CE Demo Application (see License.txt)

	FILE: oSceneLibDemo.h

	DESCRIPTION: Scene Loader Demo header

	CREATED BY: Andres Carrera

	HISTORY: 

 *>	Copyright (c) 2006, All Rights Reserved.
 **********************************************************************/

#include "ExampleApplication.h"

// oScene loader library header file
#include "OgreOSMScene.h"

// Callback handler to post-process created objects
class oSceneCallback : public OSMSceneCallbacks {
public:

	// We override the OnCreate method for cameras (See IXMLSceneCallbacks class)
	void OnCameraCreate(Ogre::Camera* pCamera, TiXmlElement* pCameraDesc) {
	
		// If a camera of name "FirstCamera" is loaded, it will be set as the default current 
		if(pCamera->getName() == "FirstCamera")
			Ogre::Root::getSingleton().getAutoCreatedWindow()->getViewport(0)->setCamera(pCamera);
	}

};


// Event handler to animate
class oSceneLibDemoFrameListener : public ExampleFrameListener
{
protected:

	SceneManager* mSceneMgr;

public:
    oSceneLibDemoFrameListener(SceneManager* sceneMgr, RenderWindow* win, Camera* cam)
        : mSceneMgr(sceneMgr), ExampleFrameListener(win, cam)
    {
    }


    bool frameStarted(const FrameEvent& evt)
    {

		static Real timeDelay = 0;
		static Real currentTime = 0;

        timeDelay -= evt.timeSinceLastFrame;

		// We can cycle cameras with the 'c' key
		if (mInputDevice->isKeyDown(KC_C) && timeDelay <= 0) {			

			Camera* firstCam;
			Camera* currentCam = mWindow->getViewport(0)->getCamera();

			Viewport* vp = mWindow->getViewport(0);

			SceneManager::CameraIterator it = mSceneMgr->getCameraIterator();

			if(it.hasMoreElements())
				firstCam = it.peekNextValue();

			while(it.hasMoreElements()) {
				
				Ogre::Camera* cam = it.getNext();

				if(currentCam == cam) {
					Ogre::Camera* camera = it.hasMoreElements() ? it.getNext() : firstCam;
					vp->setCamera(camera);					
				}
			}

			timeDelay = 0.5f;
			
		}

		// We update all loaded animations each frame
		SceneManager::AnimationIterator animationIt = mSceneMgr->getAnimationIterator();

		while(animationIt.hasMoreElements()) {
			Animation* animation = animationIt.getNext();
			
			const Animation::NodeTrackList& trackList = animation->_getNodeTrackList();
			
			Animation::NodeTrackList::const_iterator it = trackList.begin();
			Animation::NodeTrackList::const_iterator iend = trackList.end();

			for(; it != iend; ++it) {
				const Ogre::NodeAnimationTrack* track = it->second;
				track->getAssociatedNode()->resetToInitialState();
			}
			
			currentTime += evt.timeSinceLastFrame;
			animation->apply(currentTime);
		}		



        // Call default
        return ExampleFrameListener::frameStarted(evt);

    }
};

class oSceneLibApplication : public ExampleApplication {
public:
	
	oSceneLibApplication() {}

protected:

	// Just override the mandatory create scene method
    void createScene(void)
    {
		// Here is the code that will load the scene file 
		// A fixed filename "scene.osm" is implemented for this demo

		// Create the scene loader
		OSMScene oScene;

		// Create an oE_Loader Callback object to post-process created objects

		oSceneCallback oe_Callback;

		// Initialises with the scene to be loaded and the callback if requiered
		oScene.initialise("scene.osm", &oe_Callback);

		// create and setup the scene in the root node
		oScene.createScene();


		mSceneMgr = oScene.getSceneManager();

		// Get the list of cameras loaded with the scene
		OSMScene::CameraList camList = oScene.getCameraList();
		
		if(!camList.empty()) {

			// If loaded with the scene, set the first camera as the default camera
			mCamera = camList[0];

			// A viewport has been created and assigned to the first camera automatically
			// (The TSM needs it to initialize the terrain world geometry)
		}
		else {
			
			// Create a default camera in case no cameras were saved in the scene

			mCamera = mSceneMgr->createCamera("PlayerCam");
			// Position it at 500 in Z direction
			mCamera->setPosition(Vector3(0,0,500));
			// Look back along -Z
			mCamera->lookAt(Vector3(0,0,-300));
			mCamera->setNearClipDistance(5);
			
			// If a viewport was not automatically created, (no cameras saved in the scene)
			// create a default viewport, entire window
			Viewport* vp = mWindow->addViewport(mCamera);
			vp->setBackgroundColour(ColourValue(0,0,0));

			// Alter the camera aspect ratio to match the viewport
			mCamera->setAspectRatio(
				Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
		}

    }

	// The scene manager will be created by the Scene Loader lib
	void chooseSceneManager(void) {}
	void createCamera(void) {}
	void createViewports(void) {}

    // Create new frame listener
    void createFrameListener(void)
    {
        mFrameListener= new oSceneLibDemoFrameListener(mSceneMgr, mWindow, mCamera);
        mRoot->addFrameListener(mFrameListener);
    }

};


