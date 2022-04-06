#pragma once
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "SoundDevice.h"
#include "../renderer/Log.h"

class SFX
{
private:
	SoundDevice* SFX_Speaker = SoundDevice::get();
	SoundSource SFX_Sound;
public:
	void Play_sound(const ALuint Sound_thing);
	void Stop_sound(const ALuint Sound_thing);
};

