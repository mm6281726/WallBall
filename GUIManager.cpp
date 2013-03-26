//GUIManager.cpp

#include "GUIManager.h"

GUIManager GUIManager::GUIControl;

    GUIManager::GUIManager() {}

    GUIManager::~GUIManager() {
        mTrayMgr->destroyAllWidgets();
    }

    void GUIManager::createScoreBoard(){
        // create a params panel for displaying score details
        Ogre::StringVector items;    
        items.push_back("Score");
        items.push_back("");
        items.push_back("");
        items.push_back("");
        items.push_back("");
        items.push_back("");
        items.push_back("Power Up");
        items.push_back("");
        items.push_back("");
        items.push_back("Filtering");
        items.push_back("Poly Mode");
        mScoreBoard = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "ScoreBoard", 200, items);
        mScoreBoard->setParamValue(9, "Bilinear");
        mScoreBoard->setParamValue(10, "Solid");
        mScoreBoard->hide();
    }

    void GUIManager::createMainScreen(){
        mTrayMgr->createButton(OgreBites::TL_CENTER, "Singleplayer", "Singleplayer", 250);
        mTrayMgr->createButton(OgreBites::TL_CENTER, "Multiplayer", "Multiplayer", 250);
        mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Exit", 250);
    }

    void GUIManager::createPauseScreen(){
        OgreBites::Button* resume = mTrayMgr->createButton(OgreBites::TL_CENTER, "Resume", "Resume", 250);
        OgreBites::Button* main = mTrayMgr->createButton(OgreBites::TL_CENTER, "MainMenu", "Main Menu", 250);
        OgreBites::Button* quit = mTrayMgr->createButton(OgreBites::TL_CENTER, "PauseExit", "Exit", 250);
        //resume->hide();
        //main->hide();
        //quit->hide();
    }

    void GUIManager::setup(OgreBites::SdkTrayManager* trayMgr){
	    mTrayMgr = trayMgr;

        createScoreBoard();
        createMainScreen();
        createPauseScreen();

        Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();

    }

    void GUIManager::begin_mainScreen(){
        //Set up Main Menu   
	    mTrayMgr->showCursor();
        mTrayMgr->showBackdrop("Examples/Chrome");
        mTrayMgr->getWidget("Singleplayer")->show();
        mTrayMgr->getWidget("Multiplayer")->show();
        mTrayMgr->getWidget("Exit")->show();
    }

    void GUIManager::end_mainScreen(){
        mTrayMgr->getWidget("Singleplayer")->hide();
        mTrayMgr->getWidget("Multiplayer")->hide();
        mTrayMgr->getWidget("Exit")->hide();
        mScoreBoard->show();
    }

    void GUIManager::inPlay(bool inPlay, int effect, Ogre::Timer timer, int score){
		if(inPlay){
        	mScoreBoard->setParamValue(0, Ogre::StringConverter::toString((timer.getMilliseconds()/100)+score));
		    mScoreBoard->setParamValue(1, "");
		}else
            mScoreBoard->setParamValue(1, "GAME OVER");

        mScoreBoard->setParamValue(2, "Bounce the ball for");
        mScoreBoard->setParamValue(3, "as long as possible");
        mScoreBoard->setParamValue(4, "to win!");
        mScoreBoard->setParamValue(6, "");

	    if(effect<=1)
          	mScoreBoard->setParamValue(6, "Accelerate");
	    else if(effect>=2)
        	mScoreBoard->setParamValue(6, "Slow");

	    mScoreBoard->setParamValue(7, "Press Enter to Restart");
    }

    bool GUIManager::isScoreboardVisible(){
        return mScoreBoard->isVisible();
    }

    Ogre::DisplayString GUIManager::getScoreboardParam(unsigned int i){
        return mScoreBoard->getParamValue(i);
    }

    void GUIManager::setScoreboardParam(unsigned int i, Ogre::String newVal){
        mScoreBoard->setParamValue(i, newVal);
    }

    bool GUIManager::isDialogVisible(){
        return mTrayMgr->isDialogVisible();
    }

    void GUIManager::frameRenderingQueued(const Ogre::FrameEvent& evt){
        mTrayMgr->frameRenderingQueued(evt);
        mTrayMgr->adjustTrays();
    }