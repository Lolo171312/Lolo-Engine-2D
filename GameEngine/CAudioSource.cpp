#include "CAudioSource.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "AudioBuffer.h"

CAudioSource::CAudioSource()
{
    //Disable isActive
    //AudioSource does not need to execute any logic every tick
    _isActive = false;

    //Generate Source
	alGenSources(1, &_sourceId);
    
    /*
    * Prepare initial values
    */
    alSourcei(_sourceId, AL_LOOPING, 0); //No Looping
    alSourcef(_sourceId, AL_PITCH, 1.0f); //Normal pitch
    alSourcef(_sourceId, AL_GAIN, 1.0f); //Normal Volume
    alSourcef(_sourceId, AL_MIN_GAIN, 0.0f); //Normal Volume
    alSourcef(_sourceId, AL_MAX_GAIN, 10.0f); //Normal Volume
    
    alSource3f(_sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f); //Zero Position
    alSource3f(_sourceId, AL_VELOCITY, 0.0f, 0.0f, 0.0f); //Zero Velocity
}

CAudioSource::~CAudioSource()
{
    alDeleteSources(1, &_sourceId);
}

void CAudioSource::SetAudioClip(AudioBuffer* newClip) const
{
    if (!newClip) return;

    alSourcei(_sourceId, AL_BUFFER, newClip->GetBufferId());
}

void CAudioSource::Play() const
{
    alSourcePlay(_sourceId);
}

void CAudioSource::Pause() const
{
    alSourcePause(_sourceId);
}

void CAudioSource::Stop() const
{
    alSourceStop(_sourceId);
}

void CAudioSource::SetPitch(const float newPitch) const
{
    alSourcef(_sourceId, AL_PITCH, newPitch);
}

void CAudioSource::SetVolume(const float newVol) const
{
    alSourcef(_sourceId, AL_GAIN, newVol);
}