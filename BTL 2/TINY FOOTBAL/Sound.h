#pragma once
#include "Game.h"

class Sound
{
public:
	Mix_Music* music_playing;
	Mix_Music* music_background;
	Mix_Chunk* goal;
	Mix_Chunk* shoot;
	Mix_Chunk* win;
	Mix_Chunk* click;

	Sound();
	~Sound();

	Sound(const Sound&) = delete;
	Sound(Sound&&) = delete;
	Sound& operator=(const Sound&) = delete;
	Sound& operator=(Sound&&) = delete;

	void changeState();
};