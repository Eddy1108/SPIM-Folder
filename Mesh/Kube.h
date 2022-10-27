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
	Kube(Shader& shader);
	Kube(Shader& shader, float size, float x, float y, float z);
	~Kube() override;
	void init() override;
	void draw() override;
};
