//SoundManager.cpp
#include "SoundManager.h"

SoundManager SoundManager::SoundControl;

SoundManager::SoundManager(){}

int SoundManager::loadWAV(char* file){
	Mix_Chunk* TempSound = NULL;
 
  if((TempSound = Mix_LoadWAV("sound/" + file)) == NULL) {
      return -1;
  }
 
  SoundList.push_back(TempSound);
 
  return (SoundList.size() - 1);
}

void SoundManager::playAudio(){
	SDL_PauseAudio(false);
}

void SoundManager::pauseAudio(){
	SDL_PauseAudio(true);
}

void SoundManager::playClip(int clipID, int loop){
	if(clipID < 0 || clipID >= SoundList.size()) return;
    if(SoundList[clipID] == NULL) return;
 
    Mix_PlayChannel(-1, SoundList[ID], -(loop));
}

void SoundManager::mixAudio(void *unused, Uint8 *stream, Uint32 len){
   	int i;
   	Uint32 amount;

   	for ( i=0; i<NUM_SOUNDS; ++i ) {
       	amount = (SoundList[i].dlen-SoundList[i].dpos);
       	if ( amount > len ) {
           	amount = len;
       	}
       	SDL_MixAudio(stream, &SoundList[i].data[SoundList[i].dpos], amount, SDL_MIX_MAXVOLUME);
       	SoundList[i].dpos += amount;
   	}
}

void SoundManager::cleanup() {
	for(int i = 0;i < SoundList.size();i++) {
        Mix_FreeChunk(SoundList[i]);
    }
 
    SoundList.clear();
}
