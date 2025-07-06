#pragma once

#include "BaseFactory.h"
#include "../SingletonBase.h"

struct AudioClipType
{
	unsigned int _bufferId;
};

class AudioClipFactory : public SingletonBase<AudioClipFactory>, public BaseFactory<AudioClipType>
{
public:
	virtual void DestroyFactory();

private:
	virtual AudioClipType* InsertNewType(const char* key);

	int16_t FloatToPCM16(float f);

	AudioClipType* LoadClip(const char* audioFileDir);
};

