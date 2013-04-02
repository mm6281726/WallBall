//Powerup.h

class Powerup{
	private:
		Ogre::Entity* mEntity;
		int effect;
	public:
		Ogre::SceneNode* mSceneNode;
		Powerup::Powerup(Ogre::Entity* power, Ogre::SceneNode* powerupNode);
}