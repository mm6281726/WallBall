//BallManager.h

#ifndef _BALLMANAGER_H_
    #define _BALLMANAGER_H_

#include <sstream>
#include <string>
#include <OgreEntity.h>

class BallManager{
	public:
		BallManager();
		Ogre::SceneNode* getNode(void);
		Ogre::Vector3 getPosition(void);
		void setPosition(Ogre::Vector3 v);
		void setNode(Ogre::SceneNode* s);
//		static NetworkManager NetworkControl;
		Ogre::SceneNode* node;
		Ogre::Vector3 position;

};

#endif
