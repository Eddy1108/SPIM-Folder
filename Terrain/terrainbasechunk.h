#ifndef TERRAINBASECHUNK_H
#define TERRAINBASECHUNK_H

#include "Core/VisualObject.h"

class TerrainBaseChunk : public VisualObject
{
public:
    TerrainBaseChunk(const int seed, glm::vec2 coords, const float chunkSize = 20, const unsigned int chunkComplexity = 128, std::string materialName = "materialplane");

    void init() override;
    void draw() override;

private:
    void generateChunk(glm::vec2 coords);
    std::vector<float> generateNoiseData(glm::vec2 coords, int width, int height);


private:
    const int mSeed{20};
    const float mChunkSize{20}; // The lenght and width of each chunk in worldspace.
    const unsigned int mChunkComplexity{128}; // The amount of vertices along the width & length of the chunk.
    float mHeightOffset{2};

    glm::vec2 coords; //Corner XY coordinates of the chunk


};

#endif // TERRAINBASECHUNK_H
