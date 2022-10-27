#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "Vertex.h"
#include "VisualObject.h"

class XYZ : public VisualObject
{
private:


public:
	XYZ(Shader& shader);
	~XYZ() override;
	void init() override;
	void draw() override;

	bool bDraw{ true };
};

