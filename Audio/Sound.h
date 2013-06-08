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
	
	bool PlaySoundFromStart();
private:

	LPDIRECTSOUNDBUFFER m_Buffer;
};
}