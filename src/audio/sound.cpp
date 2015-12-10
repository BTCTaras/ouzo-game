#include "audio/sound.hpp"

///
///	CSound
///

CSound::CSound(const char *file) {
	this->LoadFromFile(file);
}

CSound::~CSound() {
	Mix_FreeChunk(m_chunk);
}

void CSound::LoadFromFile(const char *file) {
	m_chunk = Mix_LoadWAV(file);
	if (!m_chunk) {
		fprintf(stderr, "Failed to load sound file \"%s\": %s\n", file, Mix_GetError());
	}
}

void CSound::Play(sound_play_data_t &playData) {
	if (playData.channel < 0) {
		fprintf(stderr, "Negative channels not supported!!\n Use CSoundManager::GetFreeChannel.\n");
		return;
	}

	m_channel = playData.channel;
	Mix_PlayChannel(playData.channel, m_chunk, playData.loopAmount);
}

void CSound::Pause() {
	Mix_Pause(m_channel);
}

void CSound::Stop() {
	Mix_HaltChannel(m_channel);
}