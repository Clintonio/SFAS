#include "Sound.h"
#include "SoundProvider.h"

using namespace Engine;

Sound::Sound(LPDIRECTSOUNDBUFFER buffer) : m_Buffer(buffer)
{

}

Sound::~Sound()
{
	m_Buffer->Release();
}

bool Sound::PlaySoundFromStart()
{
	// Set position at the beginning of the sound buffer.
	if(FAILED(m_Buffer->SetCurrentPosition(0)))
	{
		return false;
	}
 
	// Set volume of the buffer to 100%.
	if(FAILED(m_Buffer->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}
 
	// Play the contents of the secondary sound buffer.
	if(FAILED(m_Buffer->Play(0, 0, 0)))
	{
		return false;
	}
 
	return true;
}