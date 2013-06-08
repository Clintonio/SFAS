#include "Sound.h"
#include "SoundProvider.h"

using namespace Engine;

Sound::Sound(LPDIRECTSOUNDBUFFER buffer) : m_Buffer(buffer)
{
	m_Buffer->SetVolume(DSBVOLUME_MAX);
}

Sound::~Sound()
{
	//m_Buffer->Release();
}

bool Sound::SetVolume( const float volume )
{
	// Restrict the volume modifier between min and max
	long newVolume = (long) max(DSBVOLUME_MIN, min(DSBVOLUME_MAX, DSBVOLUME_MAX * volume));
	HRESULT result = FAILED(m_Buffer->SetVolume(newVolume));
	return (result == S_OK);
}

bool Sound::PlaySoundFromStart()
{
	// Set position at the beginning of the sound buffer.
	if(FAILED(m_Buffer->SetCurrentPosition(0)))
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

bool Sound::Stop()
{
	return FAILED(m_Buffer->Stop());
}