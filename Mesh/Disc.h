#pragma once
#include "Core/VisualObject.h"

class Disc : public VisualObject
{
public:
	Disc(Shader& shader);
	Disc(Shader& shader, std::string filename);
	~Disc() override;
	
	void readFile(std::string filename);
	void writeFile(std::string filename);
	void init() override;
	void draw() override;
	void construct();
	void move(float dt) override;

protected:
	std::vector<GLuint> mIndices;	//til indeksering
	GLuint mIBO{ 0 };				// til glDrawElements();

	float mRadius{ 0.5 };
	//QVector4D mRotation;			// her eller i VisualObjects???? Bruk en av dem
};

