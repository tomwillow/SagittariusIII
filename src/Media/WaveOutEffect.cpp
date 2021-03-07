#include "WaveOutEffect.h"

#include "tstring.h"

#pragma comment(lib,"winmm.lib")

#define _PRINT
#include <stdexcept>

using namespace std;

WaveOutEffect::WaveOutEffect(PWAVEFORMATEX pWaveformat, int buf_ms) :
	m_hWaveOut(0)
{
	//初始化音频格式
	memcpy(&m_Waveformat, pWaveformat, sizeof(WAVEFORMATEX));
	m_Waveformat.nBlockAlign = (m_Waveformat.wBitsPerSample * m_Waveformat.nChannels) >> 3;
	m_Waveformat.nAvgBytesPerSec = m_Waveformat.nBlockAlign * m_Waveformat.nSamplesPerSec;

	//分配缓冲区
	buf_size = buf_ms * m_Waveformat.nSamplesPerSec * m_Waveformat.nBlockAlign / 1000;
	buf = new char[buf_size];

	//清空WAVEHDR
	ZeroMemory(&wavehdr, sizeof(WAVEHDR));

	//设置WAVEHDR
	wavehdr.lpData = buf;
	wavehdr.dwBufferLength = buf_size;

	Open();
}

WaveOutEffect::~WaveOutEffect()
{
	Reset();

	mRet = waveOutUnprepareHeader(m_hWaveOut, &wavehdr, sizeof(WAVEHDR));
	if (mRet != MMSYSERR_NOERROR)
	{
		TCHAR info[260];
		waveOutGetErrorText(mRet, info, 260);
		throw runtime_error(to_string(info));
	}

	mRet = waveOutClose(m_hWaveOut);
	if (mRet != MMSYSERR_NOERROR)
	{
		throw runtime_error("waveOutClose fail");
	}

	delete[] buf;
}

void WaveOutEffect::PlayAudio(char* in_buf, unsigned int in_bufsize)
{
	if (in_bufsize > buf_size)//传入buf大于内置缓冲区，抛出异常
	{
		throw runtime_error("input buffer size is bigger than self");
	}

	if (in_bufsize <= buf_size)
	{
		wavehdr.dwBufferLength = in_bufsize;
	}

	memcpy(buf, in_buf, in_bufsize);

	Reset();

	mRet = waveOutPrepareHeader(m_hWaveOut, &wavehdr, sizeof(WAVEHDR));
	if (mRet != MMSYSERR_NOERROR)
	{
		TCHAR info[260];
		waveOutGetErrorText(mRet, info, 260);
		throw runtime_error(to_string(info));
	}

	mRet = waveOutWrite(m_hWaveOut, &wavehdr, sizeof(WAVEHDR));
	if (mRet != MMSYSERR_NOERROR)
	{
		TCHAR info[260];
		waveOutGetErrorText(mRet, info, 260);
   		throw runtime_error(to_string(info));
	}
}

void WaveOutEffect::Open()
{
	mRet = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_Waveformat, NULL, 0, CALLBACK_NULL);
	if (mRet != MMSYSERR_NOERROR)
	{
		throw runtime_error("waveOutOpen fail");
	}
}

void WaveOutEffect::Reset()
{
	mRet = waveOutReset(m_hWaveOut);
	if (mRet != MMSYSERR_NOERROR)
	{
		TCHAR info[260];
		waveOutGetErrorText(mRet, info, 260);
		throw runtime_error(to_string(info));
	}
}
