#pragma once
#include <iostream>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <library_includes/dr_lib/dr_wav.h>


class Audio
{
public:
	Audio();
	~Audio();

private:
	unsigned int channels = 0;
	unsigned int sampleRate = 0;
	drwav_uint64 totalPCMFrameCount = 0;
	std::vector<uint16_t> pcmData;
	drwav_uint16 getTotalSamples();

	ALCdevice* device{ nullptr };
	ALCcontext* context{ nullptr };
	ALuint monoSource;
	ALuint monoSoundBuffer;

	bool bAudioSourceMade{ false };

public:
	void ReadAudioData();
	void PlayAudio();
};
