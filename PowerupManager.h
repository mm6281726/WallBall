//PowerupManager.h

#ifndef _POWERUPMANAGER_H_
    #define _POWERUPMANAGER_H_

#include <vector>
#include <Powerup.h>

class PowerupManager{
	private:
		std::vector<Powerup*> PowerupList;
		Ogre::SceneManager* mSceneMgr;
	public:
		PowerupManager();
		~PowerupManager();
		void setup(Ogre::SceneManager* mSceneMgr);
		Powerup* generatePowerup();
		void updatePowerup();
		void destroyPowerup();
		
		static PowerupManager PowerupControl;
};

#endif