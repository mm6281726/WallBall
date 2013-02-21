/*
-----------------------------------------------------------------------------
Filename:    MinimalOgre.cpp
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
#include "MinimalOgre.h"
#include <time.h>

Ogre::Vector3 velocity(0,0,0);
 
//-------------------------------------------------------------------------------------
MinimalOgre::MinimalOgre(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}
//-------------------------------------------------------------------------------------
MinimalOgre::~MinimalOgre(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;
 
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}
 
bool MinimalOgre::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
 
    // construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg);
 
//-------------------------------------------------------------------------------------
    // setup resources
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
 
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
//-------------------------------------------------------------------------------------
    // configure
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "MinimalOgre Render Window");
    }
    else
    {
        return false;
    }
//-------------------------------------------------------------------------------------
    // choose scenemanager
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
//-------------------------------------------------------------------------------------
    // create camera
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
 
    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,10,100));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
 
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
//-------------------------------------------------------------------------------------
    // create viewports
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
//-------------------------------------------------------------------------------------
    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
//-------------------------------------------------------------------------------------
    // Create any resource listeners (for loading screens)
    //createResourceListener();
//-------------------------------------------------------------------------------------
    // load resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//-------------------------------------------------------------------------------------
    // Create the scene
    Ogre::Entity* ball = mSceneMgr->createEntity("Ball", "sphere.mesh");
 
    Ogre::SceneNode* ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallNode");
    ballNode->attachObject(ball);
    ballNode->scale(.1,.1,.1);
    
    srand ( time(NULL) );
    velocity.x = rand() % 10*20;
    velocity.y = rand() % 10*20;
    velocity.z = rand() % 10*20;

    Ogre::Plane roofFront(Ogre::Vector3::NEGATIVE_UNIT_Y, -100);
    Ogre::Plane roofBack(Ogre::Vector3::UNIT_Y, 100);
    Ogre::Plane floorFront(Ogre::Vector3::UNIT_Y, -100);
    Ogre::Plane floorBack(Ogre::Vector3::NEGATIVE_UNIT_Y, 100);
    Ogre::Plane rightWallFront(Ogre::Vector3::NEGATIVE_UNIT_X, -100);
    Ogre::Plane rightWallBack(Ogre::Vector3::UNIT_X, 100);
    Ogre::Plane leftWallFront(Ogre::Vector3::UNIT_X, -100);
    Ogre::Plane leftWallBack(Ogre::Vector3::NEGATIVE_UNIT_X, 100);
    Ogre::Plane backWallFront(Ogre::Vector3::NEGATIVE_UNIT_Z, -300);
    Ogre::Plane backWallBack(Ogre::Vector3::UNIT_Z, 300);
    Ogre::Plane frontWallFront(Ogre::Vector3::UNIT_Z, -300);
    Ogre::Plane frontWallBack(Ogre::Vector3::NEGATIVE_UNIT_Z, 300);
    
    Ogre::MeshManager::getSingleton().createPlane("roofFront", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    roofFront, 200, 600, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);   
    Ogre::MeshManager::getSingleton().createPlane("roofBack", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    roofBack, 200, 600, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("floorFront", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    floorFront, 200, 600, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("floorBack", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    floorBack, 200, 600, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("rightWallFront", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    rightWallFront, 600, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("rightWallBack", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    rightWallBack, 600, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("leftWallFront", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    leftWallFront, 600, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("leftWallBack", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    leftWallBack, 600, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("backWallFront", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    backWallFront, 200, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
    Ogre::MeshManager::getSingleton().createPlane("backWallBack", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    backWallBack, 200, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
    Ogre::MeshManager::getSingleton().createPlane("frontWallFront", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    frontWallFront, 200, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
    Ogre::MeshManager::getSingleton().createPlane("frontWallBack", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    frontWallBack, 200, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X); 

    Ogre::Entity* entRoofFront = mSceneMgr->createEntity("RoofFrontEntity", "roofFront");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entRoofFront);
    entRoofFront->setMaterialName("Examples/Rockwall");
    entRoofFront->setCastShadows(false);

    Ogre::Entity* entRoofBack = mSceneMgr->createEntity("RoofBackEntity", "roofBack");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entRoofBack);
    entRoofBack->setMaterialName("Examples/Rockwall");
    entRoofBack->setCastShadows(false);

    Ogre::Entity* entFloorFront = mSceneMgr->createEntity("FloorFrontEntity", "floorFront");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entFloorFront);
    entFloorFront->setMaterialName("Examples/Rockwall");
    entFloorFront->setCastShadows(false);

    Ogre::Entity* entFloorBack = mSceneMgr->createEntity("FloorBackEntity", "floorBack");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entFloorBack);
    entFloorBack->setMaterialName("Examples/Rockwall");
    entFloorBack->setCastShadows(false);

    Ogre::Entity* entRightWallFront = mSceneMgr->createEntity("RightWallFront", "rightWallFront");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entRightWallFront);
    entRightWallFront->setMaterialName("Examples/Rockwall");
    entRightWallFront->setCastShadows(false);

    Ogre::Entity* entRightWallBack = mSceneMgr->createEntity("RightWallBack", "rightWallBack");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entRightWallBack);
    entRightWallBack->setMaterialName("Examples/Rockwall");
    entRightWallBack->setCastShadows(false);

    Ogre::Entity* entLeftWallFront = mSceneMgr->createEntity("LefttWallFront", "leftWallFront");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entLeftWallFront);
    entLeftWallFront->setMaterialName("Examples/Rockwall");
    entLeftWallFront->setCastShadows(false);

    Ogre::Entity* entLeftWallBack = mSceneMgr->createEntity("LeftWallBack", "leftWallBack");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entLeftWallBack);
    entLeftWallBack->setMaterialName("Examples/Rockwall");
    entLeftWallBack->setCastShadows(false);

    Ogre::Entity* entBackWallFront = mSceneMgr->createEntity("BackWallFront", "backWallFront");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entBackWallFront);
    entBackWallFront->setMaterialName("Examples/Rockwall");
    entBackWallFront->setCastShadows(false);

    Ogre::Entity* entBackWallBack = mSceneMgr->createEntity("BackWallBack", "backWallBack");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entBackWallBack);
    entBackWallBack->setMaterialName("Examples/Rockwall");
    entBackWallBack->setCastShadows(false);

    Ogre::Entity* entFrontWallFront = mSceneMgr->createEntity("FrontWallFront", "frontWallFront");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entFrontWallFront);
    entFrontWallFront->setMaterialName("Examples/Rockwall");
    entFrontWallFront->setCastShadows(false);

    Ogre::Entity* entFrontWallBack = mSceneMgr->createEntity("FrontWallBack", "frontWallBack");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entFrontWallBack);
    entFrontWallBack->setMaterialName("Examples/Rockwall");
    entFrontWallBack->setCastShadows(false);

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
    // Create a light
    Ogre::Light* spotLight1 = mSceneMgr->createLight("spotLight1");
    spotLight1->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight1->setDiffuseColour(0, 0, 1);
    spotLight1->setSpecularColour(0, 0, 1);
    spotLight1->setDirection(0, -1, 0);
    spotLight1->setPosition(0,15,0);

    Ogre::Light* spotLight2 = mSceneMgr->createLight("spotLight2");
    spotLight2->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight2->setDiffuseColour(1, 0, 0);
    spotLight2->setSpecularColour(1, 0, 0);
    spotLight2->setDirection(0, 1, 0);
    spotLight2->setPosition(0,-15,0);

    Ogre::Light* spotLight3 = mSceneMgr->createLight("spotLight3");
    spotLight3->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight3->setDiffuseColour(0, 1, 0);
    spotLight3->setSpecularColour(0, 1, 0);
    spotLight3->setDirection(-1, 0, 0);
    spotLight3->setPosition(15,0,0);

    Ogre::Light* spotLight4 = mSceneMgr->createLight("spotLight4");
    spotLight4->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight4->setDiffuseColour(.5, .5, .5);
    spotLight4->setSpecularColour(.5, .5, .5);
    spotLight4->setDirection(1, 0, 0);
    spotLight4->setPosition(-15,0,0);

    Ogre::Light* spotLight5 = mSceneMgr->createLight("spotLight5");
    spotLight5->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight5->setDiffuseColour(1, 1, 0);
    spotLight5->setSpecularColour(1, 1, 0);
    spotLight5->setDirection(0, 0, -1);
    spotLight5->setPosition(0,0,15);

    Ogre::Light* spotLight6 = mSceneMgr->createLight("spotLight6");
    spotLight6->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight6->setDiffuseColour(0, 1, 1);
    spotLight6->setSpecularColour(0, 1, 1);
    spotLight6->setDirection(0, 0, 1);
    spotLight6->setPosition(0,0,-15);
    
    ballNode->attachObject(spotLight1);
    ballNode->attachObject(spotLight2);
    ballNode->attachObject(spotLight3);
    ballNode->attachObject(spotLight4);
    ballNode->attachObject(spotLight5);
    ballNode->attachObject(spotLight6);
    
//-------------------------------------------------------------------------------------
    //create FrameListener
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();
 
    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");
 
    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
 
    mRoot->addFrameListener(this);
//-------------------------------------------------------------------------------------
    mRoot->startRendering();
 
    return true;
}
 
bool MinimalOgre::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    if(mShutDown)
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
 
    mTrayMgr->frameRenderingQueued(evt);
 
    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }
    Ogre::SceneNode* ball = mSceneMgr->getSceneNode("BallNode");
    Ogre::Vector3 ballPosition = mSceneMgr->getSceneNode("BallNode")->getPosition();
    if(ballPosition.x>90) {
        ballPosition.x = 90;
        velocity.x *= -1;
    } else if(ballPosition.x<-90) {
        ballPosition.x = -90;
        velocity.x *= -1;
    }
    if(ballPosition.y>90) {
        ballPosition.y=90;
        velocity.y *= -1;
    } else if(ballPosition.y<-90) {
        ballPosition.y = -90;
        velocity.y *= -1;
    }
    if(ballPosition.z>90) {
        ballPosition.z = 90;
        velocity.z *= -1;
    } else if(ballPosition.z<-90) {
        ballPosition.z = -90;
        velocity.z *= -1;
    }

    ball->setPosition(ballPosition);
    ball->translate(evt.timeSinceLastFrame*velocity);
    ball->pitch( Ogre::Degree( evt.timeSinceLastFrame*velocity.x ) );
    ball->yaw( Ogre::Degree( evt.timeSinceLastFrame*velocity.y ) );
    ball->roll( Ogre::Degree( evt.timeSinceLastFrame*velocity.z ) );
    //mCamera->lookAt(mSceneMgr->getSceneNode("BallNode")->getPosition());
    
    return true;
}
//-------------------------------------------------------------------------------------
bool MinimalOgre::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
 
    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle texture filtering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;
 
        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }
 
        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;
 
        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }
 
        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
 
    mCameraMan->injectKeyDown(arg);
    return true;
}
 
bool MinimalOgre::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}
 
bool MinimalOgre::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
    mCameraMan->injectMouseMove(arg);
    return true;
}
 
bool MinimalOgre::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}
 
bool MinimalOgre::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
}
 
//Adjust mouse clipping area
void MinimalOgre::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
//Unattach OIS before window shutdown (very important under Linux)
void MinimalOgre::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
 
 
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        MinimalOgre app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
