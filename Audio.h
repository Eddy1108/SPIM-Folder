#pragma once
#include <iostream>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <dr_lib/dr_wav.h>
#include "glm/glm.hpp"

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
	//void PlayAudio();
	void PlayAudio(float gain = 1.f, float pitch = 1.f, bool loop = 1, glm::vec3 pos = glm::vec3{0.f,0.f,0.f}, glm::vec3 velocity = glm::vec3{0.f,0.f,0.f});
	void UpdateAudio(float gain = 1.f, float pitch = 1.f, glm::vec3 pos = glm::vec3{ 0.f,0.f,0.f }, glm::vec3 velocity = glm::vec3{ 0.f,0.f,0.f });
	void UpdateListener(glm::vec3 CamPos, glm::vec3 CamVel, glm::vec3 CamUpVector, glm::vec3 CamForwardVector);
};

