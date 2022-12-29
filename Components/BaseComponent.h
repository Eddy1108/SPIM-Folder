#pragma once
#include <QOpenGLFunctions_4_1_Core>

class VisualObject;

class BaseComponent : public QOpenGLFunctions_4_1_Core
{
public:
	BaseComponent(VisualObject* parent, bool isActivatedFromStart = true);

	virtual void init() { ; };	//Initialize once
	virtual void update() { ; };	//update each frame (draw)
	virtual void activate() { ; }; //function that can be used for anything e.g. activate the component

	void setName(std::string name) { mComponentName = name; };

	bool bIsActive{ true };

	std::string mComponentName;

protected:
	VisualObject* mParent{ nullptr };
};

