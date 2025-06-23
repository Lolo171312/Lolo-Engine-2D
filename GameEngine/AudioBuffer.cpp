#include "AudioBuffer.h"
#include <fstream>
#include <iostream>
#include "AL/alc.h"
#include "AL/al.h"
#include "Helpers.h"

AudioBuffer* AudioBuffer::Load(const char* audioFileDir)
{
	std::ifstream file(audioFileDir, std::ios::binary);
	if (!file.is_open()) 
	{
		std::cout << "ERROR: Could not find AudioBuffer" << std::endl;
		return nullptr;
	}

	char readingChunk[4];
	file.read(readingChunk, 4); //ChunkID("RIFF")
	if (strncmp(readingChunk, "RIFF", 4) != 0)
	{
		std::cout << "ERROR: The clip was in an incorrect format" << std::endl;
		return nullptr;
	}

	file.read(readingChunk, 4); //RiffChunkSize
	file.read(readingChunk, 4); //Format("wave)
	file.read(readingChunk, 4); //SubChunkID ("fmt")
	file.read(readingChunk, 4); //FmtChunkSize
	int fmtChunkSize = ConvertBytesToInt(readingChunk, 4);
	file.read(readingChunk, 2); //AudioFormat
	int audioFormat = ConvertBytesToInt(readingChunk, 2);
	file.read(readingChunk, 2); //Channels
	int channels = ConvertBytesToInt(readingChunk, 2);
	file.read(readingChunk, 4); //SampleRate
	int frequency = ConvertBytesToInt(readingChunk, 4);
	file.read(readingChunk, 4); //ByteRate
	file.read(readingChunk, 2); //BlockAlign
	file.read(readingChunk, 2); //BitsPerSample
	int bitsPerSample = ConvertBytesToInt(readingChunk, 2);
	
	if(audioFormat != 1)
	{
		if (fmtChunkSize > 16) 
		{
			file.read(readingChunk, 2); //ExtraParamsSize
			int extraParamsSize = ConvertBytesToInt(readingChunk, 2);
			file.read(readingChunk, extraParamsSize); //ExtraParams
		}
	}

	int securityExit = 0;
	while (strncmp(readingChunk, "data", 4) != 0)
	{
		file.read(readingChunk, 4);
		++securityExit;
		if (securityExit > 1000) 
		{
			std::cout << "ERROR: Could not read audio data" << std::endl;
			return nullptr;
		}
	}

	file.read(readingChunk, 4);
	int dataSize = ConvertBytesToInt(readingChunk, 4);

	char* data = new char[dataSize];
	file.read(data, dataSize);

	unsigned int buffer;
	alGenBuffers(1, &buffer);

	ALenum format = 0;
	if (bitsPerSample == 8) 
	{
		if(channels == 1)
			format = AL_FORMAT_MONO8;
		else
			format = AL_FORMAT_STEREO8;
	}
	else if(bitsPerSample == 16)
	{
		if (channels == 1) 
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
	}

	alBufferData(buffer, format, data, dataSize, frequency);

	AudioBuffer* newAudioBuffer = new AudioBuffer(buffer);
	return newAudioBuffer;
}