#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// sf::
#include <SFML/Audio.hpp> // Sound

class SoundPlayer {
public:
	SoundPlayer();
	void update();
	void play();

private:
	sf::Sound sound;
	sf::SoundBuffer buffer;
	clock_t clockPlay;
};
