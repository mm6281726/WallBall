//SoundManager.h

#ifndef _SOUNDMANAGER_H_
    #define _SOUNDMANAGER_H_


#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"
//#include "SDL/SDL_mixer.h"
#include <string>
#include <vector>

#define NUM_SOUNDS 2
struct sample {
	SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} SoundList[NUM_SOUNDS];

class SoundManager{
	private:
		SoundManager(void);
		int size;
		//std::vector<Sound_Chunk*> SoundList;
	public:
		int loadWAV(char nym[32]);
		void playAudio();
		void pauseAudio();
		void playClip(int clipID, bool loop);
		void mixAudio(void *unused, Uint8 *stream, Uint32 len);
		void cleanup();
		static SoundManager getInstance();
};

#endif