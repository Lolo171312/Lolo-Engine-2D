#define DR_WAV_IMPLEMENTATION
#include "AudioClipFactory.h"
#include <fstream>
#include <iostream>
#include "AL/alc.h"
#include "AL/al.h"
#include "../Helpers.h"
#include "../Audio/dr_wav.h"

void AudioClipFactory::DestroyFactory()
{
	delete AudioClipFactory::GetInstance();
}

AudioClipType* AudioClipFactory::InsertNewType(const char* key)
{
	return LoadClip(key);
}

int16_t AudioClipFactory::FloatToPCM16(float f)
{
    f = std::fmaxf(-1.0f, std::fminf(1.0f, f));
    return static_cast<int16_t>(f * 32767.0f);
}

AudioClipType* AudioClipFactory::LoadClip(const char* audioFileDir)
{
    drwav wav;
    if (!drwav_init_file(&wav, audioFileDir, nullptr)) {
        std::cout << "ERROR: Failed to load WAV: " << audioFileDir << std::endl;
        return nullptr;
    }

    std::vector<int16_t> pcmData;

    if (wav.translatedFormatTag == DR_WAVE_FORMAT_PCM && wav.bitsPerSample == 16) {
        pcmData.resize(wav.totalPCMFrameCount * wav.channels);
        drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pcmData.data());

    }
    else if (wav.translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT && wav.bitsPerSample == 32) {
        std::vector<float> floatData(wav.totalPCMFrameCount * wav.channels);
        drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, floatData.data());

        pcmData.resize(floatData.size());
        for (size_t i = 0; i < floatData.size(); ++i)
            pcmData[i] = FloatToPCM16(floatData[i]);

    }
    else if (wav.bitsPerSample == 24) {
        // Manually read 24-bit samples and convert to int16
        size_t totalSamples = wav.totalPCMFrameCount * wav.channels;
        std::vector<uint8_t> rawData(totalSamples * 3);
        drwav_read_raw(&wav, rawData.size(), rawData.data());

        pcmData.resize(totalSamples);
        for (size_t i = 0, j = 0; i < totalSamples; ++i, j += 3) {
            int32_t sample = (rawData[j + 2] << 16) | (rawData[j + 1] << 8) | rawData[j];
            if (sample & 0x800000) sample |= ~0xFFFFFF; // Sign extend
            pcmData[i] = static_cast<int16_t>(sample >> 8); // Downscale
        }

    }
    else {
        std::cout << "ERROR: Unsupported WAV format (bitsPerSample = " << wav.bitsPerSample << ")\n";
        drwav_uninit(&wav);
        return nullptr;
    }

    drwav_uninit(&wav);

    ALenum format = 0;
    if (wav.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (wav.channels == 2)
        format = AL_FORMAT_STEREO16;
    else {
        std::cout << "ERROR: Only mono/stereo supported (channels = " << wav.channels << ")\n";
        return nullptr;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, pcmData.data(), pcmData.size() * sizeof(int16_t), wav.sampleRate);

    auto* clip = new AudioClipType();
    clip->_bufferId = buffer;
    return clip;
}
