#pragma once

#include "audio/sound.hpp"

#include <SDL2/SDL_mixer.h>
#include <vector>

class CSoundManager {
public:
	CSoundManager(unsigned int channels);
	~CSoundManager();

	unsigned int AllocateChannel();
	void FreeChannel(unsigned int channel);

	void PlayMusic(const char *file);
	void StopMusic();
	void PauseMusic();

private:
	Mix_Music *m_music;
	bool *m_usedChannels;
	unsigned int m_channelCount;
};