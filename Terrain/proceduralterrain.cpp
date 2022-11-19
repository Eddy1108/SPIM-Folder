#include "proceduralterrain.h"
#include "Noise/FastNoiseLite.h"
#include "terrainbasechunk.h"

ProceduralTerrain::ProceduralTerrain(std::string materialName) : VisualObject(materialName)
{
    mQuadTree = new QuadTree();

    //generateChunk(glm::vec2(0,0));
    //generateChunk(glm::vec2(0,0));

    //for(int i = 0; i < 2; i++)
    //    for(int j = 0; j < 2; j++) {
    //        TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(i*mChunkSize,j*mChunkSize));
    //        mChunks.push_back(chunk);
    //

    TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*0,mChunkSize*0), BiomeType::Hills, mChunkSize, mChunkComplexity, materialName);
    mChunks.push_back(chunk);

    chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*1,mChunkSize*0), BiomeType::Desert, mChunkSize, mChunkComplexity, materialName);
    mChunks.push_back(chunk);

    chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*0,mChunkSize*1), BiomeType::Mountains, mChunkSize, mChunkComplexity, materialName);
    mChunks.push_back(chunk);

    //chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*1,mChunkSize*1), BiomeType::Hills, mChunkSize, mChunkComplexity, materialName);
    //mChunks.push_back(chunk);
}

ProceduralTerrain::~ProceduralTerrain()
{
    mChunks.clear();
}

void ProceduralTerrain::init()
{
    for(int i = 0; i < mChunks.size(); i++){
        mChunks[i]->init();
    }
}

void ProceduralTerrain::draw()
{
    for(int i = 0; i < mChunks.size(); i++){
        mChunks[i]->draw();
    }
}
