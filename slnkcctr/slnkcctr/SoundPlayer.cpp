#include "stdafx.h"

#include "SoundPlayer.h"

// std::
#include <iostream> // cerr, cout, endl

SoundPlayer::SoundPlayer(const double& frequency, const double& length)
	: length(length)
{
	const unsigned SAMPLES = 44100;
    const unsigned SAMPLE_RATE = 44100;
    const unsigned AMPLITUDE = 30000;

    sf::Int16 raw[SAMPLES];

    const double TWO_PI = 6.28318;
    const double increment = frequency / 44100;
    double x = 0;
    for (unsigned i = 0; i < SAMPLES; i++) {
        raw[i] = sf::Int16(AMPLITUDE * sin(x*TWO_PI));
        x += increment;
    }

    if (!buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
        std::cerr << "Loading failed!" << std::endl;
    }

    sound.setBuffer(buffer);
    sound.setLoop(true);
}

void SoundPlayer::update() {
	if (clock() - clockPlay > CLOCKS_PER_SEC * length) {
		sound.pause();
	}
}

void SoundPlayer::play() {
	sound.play();
	clockPlay = clock();
}
