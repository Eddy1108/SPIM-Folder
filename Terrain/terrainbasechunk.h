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
    TerrainBaseChunk(const int seed, glm::vec2 coords, BiomeType biomeType, const float chunkSize = 20, const unsigned int chunkComplexity = 128, std::string materialName = "materialplane");

    void init() override;
    void draw() override;

private:
    void generateFastNoise();
    void noiseContinentalnessTransformation();
    void noiseHeightOffsetTransformation();

    void generateChunk(glm::vec2 coords);
    float getHeight(int i, int j);
    std::vector<float> getNoiseData(FastNoiseLite* NoiseType, glm::vec2 coords, int width, int height);


private:
    const int mSeed{20};
    const float mChunkSize{20}; // The lenght and width of each chunk in worldspace.
    const unsigned int mChunkComplexity{128}; // The amount of vertices along the width & length of the chunk.
    float mHeightOffset{0};
    float mHeightIntensity{1};

    glm::vec2 mCoords; //Corner XY coordinates of the chunk

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
