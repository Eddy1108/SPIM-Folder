#ifndef TERRAINBASECHUNK_H
#define TERRAINBASECHUNK_H

#include "Core/VisualObject.h"


enum BiomeType
{
    Mountains,
    Desert,
    Hills
};

class FastNoiseLite;

class TerrainBaseChunk : public VisualObject
{
public:
    TerrainBaseChunk(const int seed, glm::vec2 position, BiomeType biomeType, const float chunkSize = 20, const unsigned int chunkComplexity = 128, std::string materialName = "materialplane", const short unsigned int LevelOfDetaial = 0);

    void init() override;
    void draw() override;

    glm::vec2 getCoords();
    glm::vec2 getPos();
    int getLOD();
    void setIndices(int LOD);
private:
    void generateFastNoise();
    void noiseContinentalnessTransformation();
    void noiseHeightOffsetTransformation();

    void generateChunk(glm::vec2 coords);
    float getHeight(int i, int j);
    std::vector<float> getNoiseData(FastNoiseLite* NoiseType, glm::vec2 coords, int width, int height);


private:
    glm::vec2 mPos; //Corner XY coordinates of the chunk

    const int mSeed{20};
    const float mChunkSize{20}; // The lenght and width of each chunk in worldspace.
    //IMPORTANT: ChunkComplexity should only be a value within the power of two + 1. 64+1, 128+1, 256+1, etc.
    unsigned int mChunkComplexity{128 + 1}; // The amount of vertices along the width & length of the chunk.
    short int mLevelOfDetail{-1}; // 0 means no reduction of detail.

    float mHeightOffset{0};
    float mContinentalIntensity{1};
    float mNoiseHeighOffsetIntensity{1};

    BiomeType mBiomeType{Mountains};

    // ---- Noise ----
    //Continental
    FastNoiseLite* mNoiseContinental{nullptr};
    std::vector<float> mNoiseContinentalData;
    //Height Offset
    FastNoiseLite* mNoiseHeightOffset{nullptr};
    std::vector<float> mNoiseHeightOffsetData;

};

#endif // TERRAINBASECHUNK_H