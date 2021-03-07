#pragma once

#include <Windows.h>
#include <mmsystem.h>

class WaveOutEffect
{
public:
	WaveOutEffect(PWAVEFORMATEX pWaveformat, int buf_ms = 80);
	~WaveOutEffect();

	void PlayAudio(char* in_buf, unsigned int in_bufsize);
private:
	char* buf;
	unsigned int buf_size;
	WAVEFORMATEX m_Waveformat;
	WAVEHDR wavehdr;
	HWAVEOUT	m_hWaveOut;
	MMRESULT mRet;

	void Open();
	void Reset();
};

