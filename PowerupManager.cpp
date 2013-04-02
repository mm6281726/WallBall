//PowerupManager.cpp

#include "PowerupManager.h"

PowerupManager PowerupManager::PowerupControl;

PowerupManager::PowerupManager(){}
PowerupManager::~PowerupManager(){
	PowerupList.clear();
}

void PowerupManager::setup(Ogre::SceneManager* sceneMgr){
	mSceneMgr = sceneMgr;
}

int PowerupManager::generatePowerup(){
	int effect = Ogre::Math::RangeRandom(0,3);
	Ogre::Entity* power;
	if(effect <= 1)
	{
		power = mSceneMgr->getEntity("PowerUpSphere");
	}
	if(effect >= 2)
	{
		power = mSceneMgr->getEntity("PowerUpSphere");
	}
	Ogre::SceneNode* powerupNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PowerUpNode");
	powerupNode->scale(0.4f,0.4f,0.4f);
	float Powx = Ogre::Math::RangeRandom(0,200)-100;
	float Powy = Ogre::Math::RangeRandom(0,200)-100;
	float Powz = Ogre::Math::RangeRandom(0,600)-300;
	powerupNode->translate(Ogre::Vector3(Powx,Powy,Powz));
	powerupNode->attachObject(power);
	powerupNode->_update(false,false);
	PowerupList.push_back(powerUp);
/*	if(effect == 3)
	{
		power = mSceneMgr->createEntity("PowerUp", "sphere.mesh");
	}
	if(effect == 4)
	{
		power = mSceneMgr->createEntity("PowerUp", "sphere.mesh");
	}*/
}

void PowerupManager::updatePowerup(){
	Ogre::SceneNode* powerupNode = mSceneMgr->getSceneNode("PowerUpNode");
	powerupNode->_update(false,false);
}

void PowerupManager::destroyPowerup(){
	mSceneMgr->destroySceneNode("PowerUpNode");
}