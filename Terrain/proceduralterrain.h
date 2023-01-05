#ifndef PROCEDURALTERRAIN_H
#define PROCEDURALTERRAIN_H

#include "Core/VisualObject.h"
#include "Core/quadtree.h"
#include <unordered_map>
//#include "Core/Camera.h"

class TerrainBaseChunk;
class Camera;

class ProceduralTerrain : public VisualObject
{
public:
    //const std::string materialName{"materialplain"};
    ProceduralTerrain(Camera* camera, const float seed, std::string materialName = "materialplain");
    ~ProceduralTerrain();
    void init();
    void draw();

    void loadChunksWithinRadius();
    glm::vec2 posToCoords(glm::vec2 position);
    float posToCoords(float position);
    bool chunkExistsAtCoords(glm::vec2 coords);
    void generateChunk(glm::vec2 coords, unsigned int levelOfDetail);

    //void generateChunk(glm::vec2 chunkCoords);
    std::vector<float> generateNoiseMapData(glm::vec2 startingCoords, int width, int height);

private:
    const int mSeed{20};
    const float mChunkSize{100}; // The lenght and width of each chunk in worldspace.
    //Important, must be a value of the Power of two + 1.   e.g. 33, 65, 129
    const unsigned int mChunkComplexity{128 + 1}; // The amount of vertices along the width & length of the chunk.
    //mipMap must be a factor of (mChunkComplexity - 1)  // 1,2,4,8,16,32,64,128.
    const int mipMap{5}; // 0 means no reduction of detail.

    const float mRenderDistance{ 800 };
    Camera* mCamera{nullptr};
    glm::vec2 mCamPosOld{0,0};

    std::unordered_map<std::string, TerrainBaseChunk*> mChunks; // Chunk coords & chunk
    QuadTree* mQuadTree{nullptr};


    bool mFirstLoad = true;

    //class FastNoiseLite* mNoise{nullptr};
};

#endif // PROCEDURALTERRAIN_H
