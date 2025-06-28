#pragma once

#include "SingletonBase.h"

class ALCdevice;
class ALCcontext;
struct AudioClipType;

class Sound : public SingletonBase<Sound>
{
public:
	Sound() : _device(nullptr), _context(nullptr)
	{}
	~Sound();

	int Initialize();

private:
	ALCcontext* _context;
	ALCdevice* _device;
};

int InitSound();
AudioClipType* LoadSoundClip(const char* clipFile);
void TerminateSound();