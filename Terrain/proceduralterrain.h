#ifndef PROCEDURALTERRAIN_H
#define PROCEDURALTERRAIN_H

#include "Core/VisualObject.h"
#include "Core/quadtree.h"
//#include "Core/Camera.h"

class TerrainBaseChunk;
class Camera;

class ProceduralTerrain : public VisualObject
{
public:
    //const std::string materialName{"materialplain"};
    ProceduralTerrain(Camera* camera, std::string materialName = "materialplain");
    ~ProceduralTerrain();
    void init();
    void draw();

    void loadChunksWithinRadius();
    glm::vec2 posToCoords(glm::vec2 position);
    float posToCoords(float position);
    bool chunkExistsAtCoords(glm::vec2 coords);

    //void generateChunk(glm::vec2 chunkCoords);
    std::vector<float> generateNoiseMapData(glm::vec2 startingCoords, int width, int height);

private:
    const int mSeed{20};
    const float mChunkSize{20}; // The lenght and width of each chunk in worldspace.
    const unsigned int mChunkComplexity{128}; // The amount of vertices along the width & length of the chunk.


    std::vector<std::pair<glm::vec2, TerrainBaseChunk*>> mChunks; // Chunk coords & chunk

    QuadTree* mQuadTree{nullptr};
    Camera* mCamera{nullptr};


    glm::vec2 mCamPosOld{0,0};

    TerrainBaseChunk* mChunkWithin{nullptr};
    float mRenderDistance{ 70 };

    bool mFirstLoad = true;

    //class FastNoiseLite* mNoise{nullptr};
};

#endif // PROCEDURALTERRAIN_H
