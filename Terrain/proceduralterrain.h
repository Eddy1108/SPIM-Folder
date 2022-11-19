#ifndef PROCEDURALTERRAIN_H
#define PROCEDURALTERRAIN_H

#include "Core/VisualObject.h"
#include "Core/quadtree.h"

class TerrainBaseChunk;

class ProceduralTerrain : public VisualObject
{
public:
    //const std::string materialName{"materialplain"};
    ProceduralTerrain(std::string materialName = "materialplain");
    ~ProceduralTerrain();
    void init();
    void draw();

    void generateChunk(glm::vec2 chunkCoords);
    std::vector<float> generateNoiseMapData(glm::vec2 startingCoords, int width, int height);

private:
    const int mSeed{20};
    const float mChunkSize{20}; // The lenght and width of each chunk in worldspace.
    const unsigned int mChunkComplexity{128}; // The amount of vertices along the width & length of the chunk.

    std::vector<TerrainBaseChunk*> mChunks;

    QuadTree* mQuadTree{nullptr};
    //class FastNoiseLite* mNoise{nullptr};

};

#endif // PROCEDURALTERRAIN_H
