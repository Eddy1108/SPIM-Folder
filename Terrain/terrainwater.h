#ifndef TERRAINWATER_H
#define TERRAINWATER_H

#include "Core/VisualObject.h"

class TerrainWater : public VisualObject
{
public:
    TerrainWater(float waterHeightOffset, glm::vec2 position, const float chunkSize, std::string materialName = "materialwater");

    void generateWater(float waterHeightOffset, glm::vec2 position, float chunkSize);

    void init() override;
    void draw() override;
};

#endif // TERRAINWATER_H
