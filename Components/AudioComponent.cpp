#include "AudioComponent.h"
#include "renderwindow.h"
#include "Core/VisualObject.h"

AudioComponent::AudioComponent(VisualObject* parent, std::string soundName, bool ActivateFromStart) : BaseComponent(parent, ActivateFromStart)
{
	mSoundPath += soundName;
	//"../SPIM-Folder/Assets/sounds/" += soundName
}

void AudioComponent::init()
{
	LoadAudio();

	//std::cout << "AUDIO Component initialized" << std::endl;
}

void AudioComponent::update()
{
	if (bIsActive)
	{
		PlayAudio();
		mAudio->UpdateAudio(1.0f,1.0f, mParent->getPosition3D());
		mAudio->UpdateListener(RenderWindow::mCurrentCamera->getPos(), glm::vec3 {0.0f,0.0f,0.0f}, RenderWindow::mCurrentCamera->GetUp(), RenderWindow::mCurrentCamera->GetForward());
	}
}

void AudioComponent::LoadAudio()
{
	mAudio = new Audio(mSoundPath);
}

void AudioComponent::PlayAudio()
{
	mAudio->PlayAudio(1.0f, 1.0f, true, mParent->getPosition3D());
}
