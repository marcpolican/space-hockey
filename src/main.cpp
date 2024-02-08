#include "gameManager.h"
#include "playState.h"
#include "menuState.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
	GameManager *game = new GameManager();

    try 
	{
        //game->start(PlayState::getInstance());
		game->start(MenuState::getInstance());
    }
	catch( Ogre::Exception& e ) 
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
        MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occured: %s\n",
                e.getFullDescription().c_str());
#endif
    }

	delete game;
    return 0;
}

