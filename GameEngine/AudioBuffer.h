#pragma once
class AudioBuffer
{
public:
	AudioBuffer(unsigned int Id) : _bufferId(Id)
	{}

	static AudioBuffer* Load(const char* audioFileDir);
	
	unsigned int GetBufferId() const { return _bufferId; }

private:
	unsigned int _bufferId;
};