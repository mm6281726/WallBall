//SoundManager.cpp
#include "SoundManager.h"

class SoundManager {

	SoundManager::SoundManager(){
		//Load SDL Library
		if(SDL_Init(SDL_INIT_AUDIO) < 0)
			std::string msg = std::string("Could'nt initialize SDL: ") + SDL_get_error();

		SDL_AudioSpec fmt;
    	/* Set 16-bit stereo audio at 44.1Khz */
    	fmt.freq = 44100;
    	fmt.format = AUDIO_S16;
    	fmt.channels = 2;
    	fmt.samples = 512;        /* A good value for games */
    	fmt.callback = mixaudio;
    	fmt.userdata = NULL;

		/* Open the audio device and start playing sound! */
    	if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        	fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        	exit(1);
    	}


	}

	int SoundManager::loadWAV(char nym[32]){
		SoundChunk* TempSound = NULL;
 
    	if((TempSound = SDL_LoadWAV(std::string("/sound") + nym)) == NULL)
        	return -1;
 
    	SoundList.push_back(TempSound);
 
    	return (SoundList.size() - 1);
	}

	void SoundManager::playAudio(){
		SDL_PauseAudio(false);
	}

	void SoundManager::pauseAudio(){
		SDL_PauseAudio(true);
	}

	void SoundManager::playMusic(char nym[32]){
		if (currentState != ERROR){
			// If no music is playing, play it
			if (Mix_PlayingMusic() == 0){
				// Load music
				try{
					Mix_Music* music = ResourceManager::getInstance()->acquireMusic(nym);
					//Play music
					Mix_PlayMusic(music, -1);
					currentState = PLAYING;
					m_sCurrentMusicFilename = nym;
				}
				catch (const Exception& e){
					std::cerr << e.what() << std::endl;
				}
			}else{
				// If music is playing, pause it
				this->pauseMusic();
			}
		}
	}

	void SoundManager::playClip(int clipID, bool loop){
		//start the clip
		if(clipID >= 0 && clipID < MAX_CLIPS)
			Mix_PlayChannel(-1, SoundList[clipID], loop);
		else
			return;
	}

	void SoundManager::mixAudio(void *unused, Uint8 *stream, Uint32 len){

    	int i;
    	Uint32 amount;

    	for ( i=0; i<NUM_SOUNDS; ++i ) {
        	amount = (sounds[i].dlen-sounds[i].dpos);
        	if ( amount > len ) {
            	amount = len;
        	}
        	SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
        	sounds[i].dpos += amount;
    	}
	}

	void SoundManager::cleanup() {
		for(int i = 0;i < SoundList.size();i++)
        	Mix_FreeChunk(SoundList[i]);
 
    	SoundList.clear();
	}

	static SoundManager* SoundManager::getInstance(){
		return SoundManager::SoundControl;
	}
}