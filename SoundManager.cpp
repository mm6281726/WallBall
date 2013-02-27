//SoundManager.cpp
#include "SoundManager.h"

#define NUM_SOUNDS 2

SoundManager::SoundManager(){
	//Load SDL Library
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
		std::string msg = std::string("Could'nt initialize SDL");

	SDL_AudioSpec fmt;
  // Set 16-bit stereo audio at 44.1Khz
  fmt.freq = 44100;
  fmt.format = AUDIO_S16;
  fmt.channels = 2;
  fmt.samples = 512;        // A good value for games
  fmt.callback = mixAudio;
  fmt.userdata = NULL;

  size = 0;

	// Open the audio device and start playing sound!
  if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    exit(1);
  }


}

int SoundManager::loadWAV(char nym[32]){
	Sound_Chunk* TempSound = NULL;

  if((TempSound = SDL_LoadWAV(std::string("/sound") + nym)) == NULL)
      return -1;
 
  SoundList[size] = TempSound;
  size++;

  return (size - 1);
}

void SoundManager::playAudio(){
	SDL_PauseAudio(false);
}

void SoundManager::pauseAudio(){
	SDL_PauseAudio(true);
}

void SoundManager::playClip(int clipID, bool loop){
	//start the clip
	if(clipID >= 0 && clipID < MAX_CLIPS)
		SDL_PlayChannel(-1, SoundList[clipID], loop);
	else
		return;
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
	for(int i = 0;i < SoundList.size();i++)
       	SDL_FreeChunk(SoundList[i]);
}
