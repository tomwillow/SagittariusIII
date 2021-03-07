#pragma once

#include <thread>
#include <memory>

#include "WaveOutEffect.h"

#include "NSFRender.h"
#ifdef _DEBUG
#pragma comment(lib,"Debug/NSFRender.lib")
#else
#pragma comment(lib,"Release/NSFRender.lib")
#endif

#include "WaveFile.h"
#include "WaveOut.h"

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void PlayBGM();
	void PlaySoundEffect();
private:
	char* buf;
	int bufsize;
	std::unique_ptr<WaveOutEffect> waveoutSelect;

	bool stopBGM;
	std::unique_ptr<NSFRender> renderBGM;
	std::unique_ptr<WaveOut> waveoutBGM;


	std::unique_ptr<WaveFile> wavefile;
	std::thread t;
	void PlayBGMThread();
};

