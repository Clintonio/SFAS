#include "SoundProvider.h"
#include "Sound.h"

#include <stdexcept>

using namespace Engine;

const DWORD SoundProvider::sDirectSoundFlags = DSSCL_NORMAL;

SoundProvider::SoundProvider()
{

}

SoundProvider::~SoundProvider()
{
	m_DirectSound->Release();
}

void SoundProvider::Init(HWND window)
{
	// Use the default device (first param is NULL)
	HRESULT result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if(FAILED(result))
	{
		switch (result)
		{
			case DSERR_ALLOCATED:
				throw std::runtime_error("Failed to create direct sound device: allocation issue");
			break;

			case DSERR_INVALIDPARAM:
				throw std::runtime_error("Failed to create direct sound device: invalid parameter");
			break;

			case DSERR_NOAGGREGATION:
				throw std::runtime_error("Failed to create direct sound device: no aggregation");
			break;

			case DSERR_NODRIVER:
				throw std::runtime_error("Failed to create direct sound device: no driver");
			break;

			case DSERR_OUTOFMEMORY:
				throw std::runtime_error("Failed to create direct sound device: outofmemory");
			break;

			default:
				throw std::runtime_error("Failed to create direct sound device");
		}
	} 

	if(FAILED(m_DirectSound->SetCooperativeLevel(window, sDirectSoundFlags)))
	{
		throw std::runtime_error("Failed to set cooperative level for sound device");
	}
}

void SoundProvider::GetWaveFormat(const WaveHeaderType &waveHeader, WAVEFORMATEX & waveFormat) const
{
	waveFormat.wFormatTag = waveHeader.audioFormat;
	waveFormat.nSamplesPerSec = waveHeader.sampleRate;
	waveFormat.wBitsPerSample = waveHeader.bitsPerSample;
	waveFormat.nChannels = waveHeader.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
}

const Sound * SoundProvider::CreateSoundBufferFromFile(const std::string file)
{
	Sound * sound = 0;

	WAVEFORMATEX waveFormat;
	LPDIRECTSOUNDBUFFER buffer;
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	
	if( LoadWaveFile( file.c_str(), &buffer, waveFormat ) )
	{
		bufferDesc.lpwfxFormat = &waveFormat;
		if(!FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &buffer, NULL)))
		{
				sound = new Sound(buffer);
		}
	}

	return sound;
}

bool SoundProvider::ValidWaveFormat(FILE* filePtr, const WaveHeaderType * waveFileHeader) const
{
	if(fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr) != 1)
	{
		return false;
	}
 
	// Check that the chunk ID is the RIFF format.
	if((waveFileHeader->chunkId[0] != 'R') || (waveFileHeader->chunkId[1] != 'I') || 
	   (waveFileHeader->chunkId[2] != 'F') || (waveFileHeader->chunkId[3] != 'F'))
	{
		return false;
	}
 
	// Check that the file format is the WAVE format.
	if((waveFileHeader->format[0] != 'W') || (waveFileHeader->format[1] != 'A') ||
	   (waveFileHeader->format[2] != 'V') || (waveFileHeader->format[3] != 'E'))
	{
		return false;
	}
 
	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader->subChunkId[0] != 'f') || (waveFileHeader->subChunkId[1] != 'm') ||
	   (waveFileHeader->subChunkId[2] != 't') || (waveFileHeader->subChunkId[3] != ' '))
	{
		return false;
	}
 
	// Check for the data chunk header.
	if((waveFileHeader->dataChunkId[0] != 'd') || (waveFileHeader->dataChunkId[1] != 'a') ||
	   (waveFileHeader->dataChunkId[2] != 't') || (waveFileHeader->dataChunkId[3] != 'a'))
	{
		return false;
	}

	return true;
}

bool SoundProvider::LoadWaveFile(const char* filename, LPDIRECTSOUNDBUFFER* secondaryBuffer, WAVEFORMATEX & waveFormat) const
{
	FILE* filePtr;
	WaveHeaderType waveFileHeader;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;
	
	if((fopen_s(&filePtr, filename, "rb") != 0) || !ValidWaveFormat(filePtr, &waveFileHeader))
	{
		return false;
	}
	
	GetWaveFormat(waveFileHeader, waveFormat);

	waveData = new unsigned char[waveFileHeader.dataSize];
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
	if(fread(waveData, 1, waveFileHeader.dataSize, filePtr) != waveFileHeader.dataSize)
	{
		return false;
	}

	if(FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);
	if(FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}
	
	fclose(filePtr);
	delete [] waveData;
 
	return true;
}