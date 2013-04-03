/*
-----------------------------------------------------------------------------
Filename:    WallBall.h
-----------------------------------------------------------------------------
 
This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __WallBall_h_
#define __WallBall_h_
 
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
 
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
 
#include <GUIManager.h>

#include <SdkCameraMan.h>

#include "Simulator.h"
#include "SoundManager.h"
#include "NetworkManager.h"

#include "SDL_net.h"
#include <sstream>
#include <string>

#include <time.h>
#include <btBulletDynamicsCommon.h>
 
class WallBall : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    WallBall(void);
    virtual ~WallBall(void);
    bool go(void);
		//PaddleManager clientPaddle, serverPaddle;
		//BallManager sharedBall;
protected:
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::Timer timer;
    bool inPlay;
    int score;

    //Powerups
    void generatePowerup(void);
    bool hasPowerUp;
    int effect;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;      // basic camera controller
    bool mCursorWasVisible;                   // was cursor visible before dialog appeared
    bool mShutDown;
 
    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    // Sounds
    int ballBounceWall;
    int ballBouncePaddle;
    int ballReset;
    int ballPowerUp;
    Mix_Music* music;

    //SDK Trays
    bool singleplayer;
    bool multiplayer;
		bool isHost;
		bool isClient;

    //Networking with SDL_net
    void NetworkHost(void);
    void NetworkClient(void);
    char* Vector3ToString(Ogre::Vector3 vic);
    Ogre::Vector3 StringToVector3(char* str);
		bool connected;
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );

    virtual void buttonHit(OgreBites::Button* button);
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
 
    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

		void setupFrontWall(void);
		void setupOpponentPaddle(void);
};
 
#endif // #ifndef __WallBall_h_
