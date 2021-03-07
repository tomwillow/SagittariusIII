#include "SoundManager.h"

#include "ExternResource.h"

using namespace std;

SoundManager::SoundManager():
	stopBGM(true)
{
	//
	wavefile = make_unique<WaveFile>();

	//
	waveoutBGM = make_unique<WaveOut>(wavefile->GetHeader());
	waveoutBGM->Start();
	renderBGM = make_unique<NSFRender>(SND_TITLE, wavefile->GetHeader());

	//
	waveoutSelect = make_unique<WaveOutEffect>(wavefile->GetHeader(), 200);
	//waveoutSelect->Start();


	unique_ptr<NSFRender> renderSelect = make_unique<NSFRender>(SND_SELECT, wavefile->GetHeader(), 200);
	renderSelect->Reset();
	renderSelect->Render();
	bufsize = renderSelect->buf_size;
	buf = new char[bufsize];
	memcpy(buf, renderSelect->buf, bufsize);
}

SoundManager::~SoundManager()
{
	stopBGM = true;
	if (t.joinable())
		t.join();

	delete[] buf;
}

void SoundManager::PlayBGM()
{
	if (!t.joinable())
		t = thread(&SoundManager::PlayBGMThread, this);
}

void SoundManager::PlaySoundEffect()
{
	waveoutSelect->PlayAudio(buf, bufsize);
}

void SoundManager::PlayBGMThread()
{
	stopBGM = false;
	while (!stopBGM)
	{
		renderBGM->Render();
		waveoutBGM->PlayAudio(renderBGM->buf, renderBGM->buf_size);
	}
}