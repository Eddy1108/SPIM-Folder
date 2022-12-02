#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "Core/Vertex.h"
#include "Core/VisualObject.h"
class Kube : public VisualObject
{
private:

public:
    Kube(std::string materialName);
    Kube(std::string materialName, float size, float x, float y, float z);
	~Kube() override;
	void init() override;
	void draw() override;
	void LoadAudio() override;
};
