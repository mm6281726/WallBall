//SoundManager.h

#ifndef _SOUNDMANAGER_H_
    #define _SOUNDMANAGER_H_

#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"
//#include "SDL/SDL_mixer.h"
#include <string>
#include <vector>

class SoundManager{
	private:
		SoundManager(void);
		static SoundManager SoundControl;
		std::vector<SoundChunk*> SoundList;
		SoundManager SoundManager::SoundControl;
	public:
		int loadWAV(char nym[32]);
		void playAudio();
		void pauseAudio();
		void playMusic(char nym[32]);
		void playClip(int clipID, bool loop);
		void mixAudio(void *unused, Uint8 *stream, Uint32 len);
		void cleanup();
		static SoundManager SoundManager::getInstance();
};

#endif