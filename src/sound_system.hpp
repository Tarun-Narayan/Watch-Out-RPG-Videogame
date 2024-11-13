#pragma once
// Sound system libraries
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>


class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	// Initialize sound system
	bool init();

	// Play music; duration = -1 for infinite loop
	void playMusic(std::string path, int duration);

	// Play sound effect
	void playSoundEffect(std::string path, int count);

private:
	Mix_Music* backgroundMusic;
};

/*
Sound credits
- Background music: https://freesound.org/people/NoiseCollector/sounds/57027/
- Arrow: 
- Boar charge: https://freesound.org/people/Robinhood76/sounds/76796/


*/