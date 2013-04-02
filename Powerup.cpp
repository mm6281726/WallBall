//Powerup.cpp

#include <Powerup.h>

Powerup::Powerup(Ogre::Entity* power, Ogre::SceneNode* powerupNode, int eff) :
	mEntity(power),
	mSceneNode(powerupNode),
	effect(eff)
{}

