#include "Audio.h"

#define OpenAL_ErrorCheck(message)\
{\
	ALenum error = alGetError();\
	if (error  != AL_NO_ERROR)\
	{\
		std::cerr << "OpenAL Error: " << error << " with call for " << #message << std::endl;\
	}\
}\

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(FUNCTION_CALL)


Audio::Audio(std::string file) : filePath{file}
{
	//Get Audio Device and use it
	const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	device = alcOpenDevice(defaultDeviceString);
	if (!device)
	{
		std::cout << "Failed to get the default audio device for OpenAL" << std::endl;
	}
	std::cout << "\nOpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	OpenAL_ErrorCheck(device);

	//Create an OpenAL audio context from device
	context = alcCreateContext(device, nullptr /*attribute list*/);

	//Activate the context
	if (!alcMakeContextCurrent(context))
	{
		std::cout << "Failed to make the OpenAL context the current context!" << std::endl;
		return;
	}
	OpenAL_ErrorCheck(context)

	//Create a listener in 3d space (the player)	(Listener always exists, you can configure the data on it)
	alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
	alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
	ALfloat forwardAndUpVectors[] = {
		//forward
		1.f, 0.f, 0.f,
		//up
		0.f,0.f,1.f
	};
	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

	ReadAudioData();

	//TODO
	//add bool for starting instantly from creation or later
}

Audio::~Audio()
{
	alec(alDeleteSources(1, &monoSource));
	alec(alDeleteBuffers(1, &monoSoundBuffer));
	alec(alcMakeContextCurrent(nullptr));
	alec(alcDestroyContext(context));
	alec(alcCloseDevice(device));
}

drwav_uint64 Audio::getTotalSamples()
{
	return totalPCMFrameCount * channels;
}

void Audio::ReadAudioData()
{
	//Read the data from file
	drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(filePath.c_str(), &channels, &sampleRate, &totalPCMFrameCount, nullptr);
	//drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16("../SPIM-Folder/Assets/sounds/WhereMono.wav", &channels, &sampleRate, &totalPCMFrameCount, nullptr);
	if (pSampleData == NULL)
	{
		std::cout << "FAILED TO LOAD AUDIO FILE" << std::endl;
		drwav_free(pSampleData, nullptr);
		return;
	}
	if (getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
	{
		std::cout << "TOO MUCH DATA IN AUDIO FILE" << std::endl;
		drwav_free(pSampleData, nullptr);
		return;
	}

	//Save the audio data
	pcmData.resize(size_t(getTotalSamples()));
	std::memcpy(pcmData.data(), pSampleData, pcmData.size() * 2);
	drwav_free(pSampleData, nullptr);

	//Generate buffer
	alec(alGenBuffers(1, &monoSoundBuffer));
	alec(alBufferData(monoSoundBuffer, channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, pcmData.data(), pcmData.size() * 2, sampleRate));
	
	std::cout << "Audio Data Loaded!" << std::endl;
}
//Start Playing Audio
//void Audio::PlayAudio()
//{
//	ALint sourceState;
//	alec(alGetSourcei(monoSource, AL_SOURCE_STATE, &sourceState));
//	if (sourceState != AL_PLAYING)
//	{
//		//Create audio source
//		alec(alGenSources(1, &monoSource));
//		alec(alSource3f(monoSource, AL_POSITION, 1.f, 0.f, 0.f));
//		alec(alSource3f(monoSource, AL_VELOCITY, 0.f, 0.f, 0.f));
//		alec(alSourcef(monoSource, AL_PITCH, 1.f));
//		alec(alSourcef(monoSource, AL_GAIN, 0.5f));
//		alec(alSourcei(monoSource, AL_LOOPING, AL_FALSE));
//		alec(alSourcei(monoSource, AL_BUFFER, monoSoundBuffer));
//
//		//Stereo
//		//ALuint stereoSource;
//		//alGenSources(1, &stereoSource);
//		////alSource3f(stereoSource, AL_POSITION, 1.f, 0.f, 0.f);
//		////alSource3f(stereoSource, AL_VELOCITY, 0.f, 0.f, 0.f);
//		//alSourcef(stereoSource, AL_PITCH, 1.f);
//		//alSourcef(stereoSource, AL_GAIN, 1.f);
//		//alSourcei(stereoSource, AL_LOOPING, AL_FALSE);
//		//alSourcei(stereoSource, AL_BUFFER, monoSoundBuffer);
//
//		std::cout << "Source Generated" << std::endl;
//
//		alec(alSourcePlay(monoSource));
//	}
//	
//}

//Start playing Audio (configurable)
void Audio::PlayAudio(float gain, float pitch, bool loop, glm::vec3 pos, glm::vec3 velocity)
{
	ALint sourceState;
	alec(alGetSourcei(monoSource, AL_SOURCE_STATE, &sourceState));
	if (sourceState != AL_PLAYING)
	{
		alec(alGenSources(1, &monoSource));
		alec(alSource3f(monoSource, AL_POSITION, pos.x, pos.y, pos.z));
		alec(alSource3f(monoSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
		alec(alSourcef(monoSource, AL_PITCH, pitch));
		alec(alSourcef(monoSource, AL_GAIN, gain));
		alec(alSourcei(monoSource, AL_LOOPING, loop));
		alec(alSourcei(monoSource, AL_BUFFER, monoSoundBuffer));

		alec(alSourcePlay(monoSource));
	}
}

//Update audio data per frame while its playing
void Audio::UpdateAudio(float gain, float pitch, glm::vec3 pos, glm::vec3 velocity)
{
	ALint sourceState;
	alec(alGetSourcei(monoSource, AL_SOURCE_STATE, &sourceState));
	if (sourceState == AL_PLAYING)
	{
		alec(alSource3f(monoSource, AL_POSITION, pos.x, pos.y, pos.z));
		alec(alSource3f(monoSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
		alec(alSourcef(monoSource, AL_PITCH, pitch));
		alec(alSourcef(monoSource, AL_GAIN, gain));
	}
}

//Update the listeners position and orientation (Use Normalized vectors)
void Audio::UpdateListener(glm::vec3 CamPos, glm::vec3 CamVel, glm::vec3 CamUpVector, glm::vec3 CamForwardVector)
{
	alec(alListener3f(AL_POSITION, CamPos.x, CamPos.y, CamPos.z));
	alec(alListener3f(AL_VELOCITY, CamVel.x, CamVel.y, CamVel.z));
	ALfloat forwardAndUpVectors[] = {
		//forward
		CamForwardVector.x, CamForwardVector.y, CamForwardVector.z,
		//up
		CamUpVector.x,CamUpVector.y,CamUpVector.z
	};
	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

	//std::cout << "CAM Pos: " << CamPos.x << ", " << CamPos.y << ", " << CamPos.z << std::endl;
}
