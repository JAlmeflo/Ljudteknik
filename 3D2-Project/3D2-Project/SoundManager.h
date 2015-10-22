#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <D3DX10math.h>

class SoundManager
{
private:
	struct WaveHeader
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
public:
	SoundManager();
	~SoundManager();

	bool Initialize(HWND p_hwnd);
	void Shutdown();

	void Update(D3DXVECTOR3 p_cameraPos);
private:
	bool InitializeDirectSound(HWND p_hwnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* p_filename, IDirectSoundBuffer8** p_secondaryBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8** p_secondaryBuffer);
	bool PlayWaveFiles();

	IDirectSound8* m_directSound;
	IDirectSoundBuffer* m_primaryBuffer;

	IDirectSoundBuffer8** m_secondaryBuffer;
	int m_nrSecondaryBuffers;
};

