#include "audio/sound_manager.hpp"

#include <limits>
#include <stdio.h>
#include <stdlib.h>

CSoundManager::CSoundManager(unsigned int channels)
	:	m_music(NULL),
		m_usedChannels(NULL),
		m_channelCount(channels)
{
	m_usedChannels = (bool*)malloc(channels * sizeof(bool));

	for (unsigned int i = 0; i < channels; ++i) {
		m_usedChannels[i] = false;
	}
}

CSoundManager::~CSoundManager() {
	if (m_usedChannels != NULL) {
		free(m_usedChannels);
	}

	if (m_music == NULL) {
		Mix_FreeMusic(m_music);
	}
}

unsigned int CSoundManager::AllocateChannel() {
	for (unsigned int i = 0; i < m_channelCount; ++i) {
		if (!m_usedChannels[i]) {
			return i;
		}
	}

	return -1;
}

void CSoundManager::FreeChannel(unsigned int channel) {
	m_usedChannels[channel] = false;
}

void CSoundManager::PlayMusic(const char *file) {
	if (m_music != NULL) {
		Mix_FreeMusic(m_music);
	}

	m_music = Mix_LoadMUS(file);

	if (!m_music) {
		fprintf(stderr, "Failed to load music file \"%s\": %s\n", file, Mix_GetError());
		return;
	}

	Mix_PlayMusic(m_music, std::numeric_limits<int>::max());
}

void CSoundManager::StopMusic() {
	Mix_HaltMusic();
	Mix_RewindMusic();
}

void CSoundManager::PauseMusic() {
	Mix_PauseMusic();
}
