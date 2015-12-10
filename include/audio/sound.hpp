#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>

struct sound_play_data_t {
	int loopAmount;
	int channel;
};

class CSound {
public:
	CSound(const char *file);
	~CSound();

	void LoadFromFile(const char *file);
	void Play(sound_play_data_t &playData);

	void Pause();
	void Stop();

private:
	int m_channel;
	Mix_Chunk *m_chunk;
};

typedef std::shared_ptr<CSound> S_CSound;