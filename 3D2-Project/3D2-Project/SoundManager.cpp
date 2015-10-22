#include "SoundManager.h"


SoundManager::SoundManager(){}
SoundManager::~SoundManager(){}

bool SoundManager::Initialize(HWND p_hwnd)
{
	m_directSound = 0;
	m_primaryBuffer = 0;
	m_nrSecondaryBuffers = 3;
	m_secondaryBuffer = new IDirectSoundBuffer8*[m_nrSecondaryBuffers];

	// Init
	if (!InitializeDirectSound(p_hwnd))
	{
		return false;
	}

	// Load sounds
	if (!LoadWaveFile("BG_music2.wav", &m_secondaryBuffer[0]))
	{
		return false;
	}
	if (!LoadWaveFile("SmokeBomb.wav", &m_secondaryBuffer[1]))
	{
		return false;
	}
	if (!LoadWaveFile("Bird.wav", &m_secondaryBuffer[2]))
	{
		return false;
	}

	// Play sounds
	if (!PlayWaveFiles())
	{
		return false;
	}

	return true;
}

void SoundManager::Shutdown()
{
	for (int i = 0; i < m_nrSecondaryBuffers; i++)
	{
		ShutdownWaveFile(&m_secondaryBuffer[i]);
	}	

	ShutdownDirectSound();
}

void SoundManager::Update(D3DXVECTOR3 p_cameraPos)
{
	D3DXVECTOR3* soundPos = new D3DXVECTOR3[m_nrSecondaryBuffers];
	soundPos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// Music
	soundPos[1] = D3DXVECTOR3(75.0f, 0.0f, 75.0f);		// Smoke
	soundPos[2] = D3DXVECTOR3(-75.0f, 0.0f, -75.0f);	// Bird

	for (int i = 0; i < m_nrSecondaryBuffers; i++)
	{
		float length = sqrt(pow(p_cameraPos.x - soundPos[i].x, 2) + pow(p_cameraPos.y - soundPos[i].y, 2) + pow(p_cameraPos.z - soundPos[i].z, 2));
		m_secondaryBuffer[i]->SetVolume(-length * 25);
	}
	
	delete[] soundPos;
}

bool SoundManager::InitializeDirectSound(HWND p_hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(NULL, &m_directSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_directSound->SetCooperativeLevel(p_hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_directSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundManager::ShutdownDirectSound()
{
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if (m_directSound)
	{
		m_directSound->Release();
		m_directSound = 0;
	}
}

bool SoundManager::LoadWaveFile(char* p_filename, IDirectSoundBuffer8** p_secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeader waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, p_filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Check format
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}


	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*p_secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeader), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	result = (*p_secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*p_secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	delete[] waveData;
	waveData = 0;
	return true;
}

void SoundManager::ShutdownWaveFile(IDirectSoundBuffer8** p_secondaryBuffer)
{
	if (*p_secondaryBuffer)
	{
		(*p_secondaryBuffer)->Release();
		*p_secondaryBuffer = 0;
	}
}

bool SoundManager::PlayWaveFiles()
{
	HRESULT result;

	for (int i = 0; i < m_nrSecondaryBuffers; i++)
	{
		result = m_secondaryBuffer[i]->SetCurrentPosition(0);
		if (FAILED(result))
		{
			return false;
		}

		result = m_secondaryBuffer[i]->SetVolume(DSBVOLUME_MAX);
		if (FAILED(result))
		{
			return false;
		}

		result = m_secondaryBuffer[i]->Play(0, 0, DSBPLAY_LOOPING);
		if (FAILED(result))
		{
			return false;
		}
	}
	return true;
}