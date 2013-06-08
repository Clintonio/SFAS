// Simple sound playback file

#pragma once

#include <DSound.h>

namespace Engine
{
class Sound
{
public:
	Sound(LPDIRECTSOUNDBUFFER buffer);
	~Sound();
	// Set the volume
	bool SetVolume( const float volume );
	bool Stop();

	bool PlaySoundFromStart();
private:

	LPDIRECTSOUNDBUFFER m_Buffer;
};
}