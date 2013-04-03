//BallManager.cpp
#include "PaddleManager.h"

PaddleManager::PaddleManager(){}

Ogre::SceneNode* PaddleManager::getNode(void)
{
	return node;
}
Ogre::Vector3 PaddleManager::getPosition(void)
{
	return position;
}
void PaddleManager::setPosition(Ogre::Vector3 v)
{
	position = v;
}
void PaddleManager::setNode(Ogre::SceneNode* s)
{
	node = s;
}
