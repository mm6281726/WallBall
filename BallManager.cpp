//BallManager.cpp
#include "BallManager.h"

BallManager::BallManager(){}

Ogre::SceneNode* BallManager::getNode(void)
{
	return node;
}
Ogre::Vector3 BallManager::getPosition(void)
{
	return position;
}
void BallManager::setPosition(Ogre::Vector3 v)
{
	position = v;
}
void BallManager::setNode(Ogre::SceneNode* s)
{
	node = s;
}
