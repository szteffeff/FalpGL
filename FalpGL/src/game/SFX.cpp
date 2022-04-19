#include "SFX.h"


void SFX::Play_sound(const ALuint Sound_thing)
{
	ALint state = false;
	if (state != AL_PLAYING) {
		SFX_Sound.Play(Sound_thing);
		//alSourcePlay(Sound_thing);
		console_log("Playing walking noise");
	}
}

void SFX::Stop_sound(const ALuint Sound_thing)
{
	//alSourceStop(Sound_thing);
	SFX_Sound.Stop();
}
