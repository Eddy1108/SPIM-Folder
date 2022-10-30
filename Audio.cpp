#include "Audio.h"

//#define OpenAL_ErrorCheck(message)\
//{\
//	ALenum error = alGetError();\
//	if (error  != AL_NO_ERROR)\
//	{\
//		std::cerr << "OpenAL Error: " << error << " with call for " << #message << std::endl;\
//	}\
//
//}\
//
//#define alec(FUNCTION_CALL)\
//FUNCTION_CALL;\
//OpenAL_ErrorCheck(FUNCTION_CALL)

Audio::Audio()
{
	//Get Audio Device and use it
	const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	device = alcOpenDevice(defaultDeviceString);
	if (!device)
	{
		std::cout << "Failed to get the default audio device for OpenAL" << std::endl;
	}
	std::cout << "\nOpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;

	//Create an OpenAL audio context from device
	context = alcCreateContext(device, nullptr /*attribute list*/);

	//Activate the context
	if (!alcMakeContextCurrent(context))
	{
		std::cout << "Failed to make the OpenAL context the current context!" << std::endl;
		return;
	}

	//Create a listener in 3d space (the player)	(Listener always exists, you can configure the data on it)
	alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
	alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
	ALfloat forwardAndUpVectors[] = {
		//forward
		1.f, 0.f, 0.f,
		//up
		0.f,0.f,1.f
	};
	alListenerfv(AL_ORIENTATION, forwardAndUpVectors);
}

Audio::~Audio()
{
	alDeleteSources(1, &monoSource);
	alDeleteBuffers(1, &monoSoundBuffer);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

drwav_uint16 Audio::getTotalSamples()
{
	return totalPCMFrameCount * channels;
}

void Audio::ReadAudioData()
{
	//Read the data from file
	drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16("../SPIM-Folder/Assets/sounds/TestSound_Mono.wav", &channels, &sampleRate, &totalPCMFrameCount, nullptr);
	//drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16("../SPIM-Folder/Assets/sounds/Vine.wav", &channels, &sampleRate, &totalPCMFrameCount, nullptr);
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
	alGenBuffers(1, &monoSoundBuffer);
	alBufferData(monoSoundBuffer, channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, pcmData.data(), pcmData.size() * 2, sampleRate);
	
	std::cout << "Audio Data Loaded!" << std::endl;
}

void Audio::PlayAudio()
{
	if (!bAudioSourceMade)
	{
		//Create audio source
		alGenSources(1, &monoSource);
		alSource3f(monoSource, AL_POSITION, 1.f, 0.f, 0.f);
		alSource3f(monoSource, AL_VELOCITY, 0.f, 0.f, 0.f);
		alSourcef(monoSource, AL_PITCH, 1.f);
		alSourcef(monoSource, AL_GAIN, 1.f);
		alSourcei(monoSource, AL_LOOPING, AL_FALSE);
		alSourcei(monoSource, AL_BUFFER, monoSoundBuffer);

			//Stereo
		//ALuint stereoSource;
		//alGenSources(1, &stereoSource);
		////alSource3f(stereoSource, AL_POSITION, 1.f, 0.f, 0.f);
		////alSource3f(stereoSource, AL_VELOCITY, 0.f, 0.f, 0.f);
		//alSourcef(stereoSource, AL_PITCH, 1.f);
		//alSourcef(stereoSource, AL_GAIN, 1.f);
		//alSourcei(stereoSource, AL_LOOPING, AL_FALSE);
		//alSourcei(stereoSource, AL_BUFFER, monoSoundBuffer);

		bAudioSourceMade = true; 
		std::cout << "Source Generated" << std::endl;

		alSourcePlay(monoSource);
	}

	//Play the sound!
	ALint sourceState;
	alGetSourcei(monoSource, AL_SOURCE_STATE, &sourceState);
	if (sourceState == AL_PLAYING)
	{
		std::cout << "\nPLAYING\n";
	}
	//else 
	//{
	//	std::cout << "\DONE PLAYING\n";
	//	// tbd
	//}

}
