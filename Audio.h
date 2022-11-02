#pragma once
#include <iostream>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <dr_lib/dr_wav.h>


class Audio
{
public:
	Audio(std::string file);
	~Audio();

private:
	unsigned int channels{ 0 };
	unsigned int sampleRate{ 0 };
	drwav_uint64 totalPCMFrameCount{ 0 };
	std::vector<uint16_t> pcmData;
	drwav_uint64 getTotalSamples();

	ALCdevice* device{ nullptr };
	ALCcontext* context{ nullptr };
	ALuint monoSource{ 0 };
	ALuint monoSoundBuffer{ 0 };

	bool bAudioSourceMade{ false };
	std::string filePath;

public:
	void ReadAudioData();
	void PlayAudio();
};

