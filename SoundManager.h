//SoundManager.h

#ifndef _SOUNDMANAGER_H_
    #define _SOUNDMANAGER_H_


#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <vector>

class SoundManager{
	private:
		static SoundManager SoundControl;
		std::vector<Mix_Chunk*> SoundList;
	public:
		SoundManager();
		int loadWAV(char* file);
		void playAudio();
		void pauseAudio();
		void playClip(int clipID, bool loop);
		void mixAudio(void *unused, Uint8 *stream, Uint32 len);
		void cleanup();
		static SoundManager getInstance();
};

#endif