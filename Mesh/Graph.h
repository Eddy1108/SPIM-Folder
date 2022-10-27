#pragma once
#include "Core/VisualObject.h"
#include "Core/Vertex.h"
#include "Kube.h"
#include <vector>

class Graph : public VisualObject
{
public:

	Graph(Shader& shader, bool b);
	Graph(Shader& shader, int graph = 1);
	~Graph();

	std::vector<QVector2D> points;
	std::vector<Kube*> Cubes;

	void init() override;
	void draw() override;
	void construct();
	void constructPoints();
	void readFile(std::string filename);
	void writeFile(std::string filename);

	bool activate(float f) override;

	float mathFunc(float x);
	float mathFunc2(float x);

protected:

	std::vector<Graph*> mGraphs;

	Graph* Graph1;
	Graph* Graph2;
	int graphUse{ 0 };

	bool bMaster{ false };
	int graphDraw{ 1 };
};

