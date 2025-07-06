#include "Block.h"
#include "../Sound.h"
#include "../CAudioSource.h"

void Block::BeginPlay()
{
	_audioSourceCmp = GetComponent<CAudioSource>();
	_breakClip = LoadSoundClip("../Content/Sounds/DestroyBlock.wav");

	if(_audioSourceCmp != nullptr)
	{
		_audioSourceCmp->SetAudioClip(_breakClip);
	}
}

void Block::OnDisable()
{
	if (_audioSourceCmp != nullptr)
	{
		_audioSourceCmp->Play();
	}
}
