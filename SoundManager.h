//SoundManager.h

#ifndef _SOUNDMANAGER_H_
    #define _SOUNDMANAGER_H_

#include <ResourceManager.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <utilities.h>
#include <string>
#include <vector>

class SoundManager{
	private:
		static SoundManager SoundControl;
		vector<Mix_Chunk*> SoundList;
	public:
		int SoundManager::loadWAV(char nym[32]);
		void SoundManager::playAudio();
		void SoundManager::pauseAudio();
		void SoundManager::playClip(int clipID, bool loop);
		void SoundManager::mixAudio(void *unused, Uint8 *stream, Uint32 len);
		void SoundManager::cleanup();
}