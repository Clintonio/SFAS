// Interface for creating simple sound object
// Using DirectSound

#pragma once

#include <dsound.h>
#include <string>
#include <map>

namespace Engine
{
class Sound;

class SoundProvider
{
public:
	SoundProvider();
	~SoundProvider();

	void Init(HWND window);

	Sound * CreateSoundBufferFromFile(const std::string file);
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	
	// Load a wave file into the given buffer
	bool LoadWaveFile(const std::string filename, LPDIRECTSOUNDBUFFER & buffer) ;
	// Create a wave format from the given file header
	void GetWaveFormat(const WaveHeaderType &waveHeader, WAVEFORMATEX & waveFormat) ;
	// Check if the header for this wave file is valid
	bool ValidWaveFormat( FILE* filePtr, WaveHeaderType & waveFileHeader) ;

	LPDIRECTSOUND8 m_DirectSound;

	static const DWORD sDirectSoundFlags;

	static std::map<std::string, Sound*> sSoundPool;

};
}