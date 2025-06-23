#pragma once
#include "LComponent.h"

class AudioBuffer;
class CAudioSource : public LComponent
{
public:
	CAudioSource();
	~CAudioSource();

	virtual void Update(float deltaTime){}

	void SetAudioClip(AudioBuffer* newClip) const;
	void Play() const;
	void Pause() const;
	void Stop() const;
	void SetPitch(const float newPitch) const;
	void SetVolume(const float newVol) const;

private:
	unsigned int _sourceId;
};

