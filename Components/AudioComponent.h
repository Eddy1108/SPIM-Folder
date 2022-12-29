#pragma once
#include "Components/BaseComponent.h"
#include "Core/Audio.h"

class AudioComponent : public BaseComponent
{
public:
	AudioComponent(VisualObject* parent, std::string soundName, bool ActivateFromStart);

	void init() override;
	void update() override;

protected:

	void LoadAudio();
	void PlayAudio();

	Audio* mAudio{ nullptr };

	std::string mSoundPath = "../SPIM-Folder/Assets/sounds/";

};

