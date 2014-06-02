#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// sf::
#include <SFML/Audio.hpp> // Sound

class SoundPlayer {
public:
	SoundPlayer(const double& frequency = 440);
	void update();
	void play();

private:
	sf::Sound sound;
	sf::SoundBuffer buffer;
	clock_t clockPlay;
};
