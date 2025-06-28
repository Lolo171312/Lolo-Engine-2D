#include "Sound.h"
#include "AL/alc.h"
#include "AL/al.h"
#include "Flyweight/AudioClipFactory.h"

Sound::~Sound()
{
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}

int Sound::Initialize()
{
    //Initialize OpenAl and open default Device
    _device = alcOpenDevice(nullptr);
    if (!_device)
    {
        return -1;
    }

    //Create OpenAl Context
    _context = alcCreateContext(_device, nullptr);
    if (!_context)
    {
        alcCloseDevice(_device);
        return -1;
    }
    //Make the created Context the current Context
    alcMakeContextCurrent(_context);

    //Modify some general values of the Listener
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_ORIENTATION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}

int InitSound()
{
    Sound* sound = Sound::Init();
    if (sound == nullptr)
    {
        return -1;
    }

    return sound->Initialize();
}

AudioClipType* LoadSoundClip(const char* clipFile)
{
    return AudioClipFactory::GetInstance()->GetType(clipFile);
}

void TerminateSound()
{
    AudioClipFactory::GetInstance()->DestroyFactory();
    delete Sound::GetInstance();
}