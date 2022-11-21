#include "proceduralterrain.h"
#include "Noise/FastNoiseLite.h"
#include "terrainbasechunk.h"

#include "renderwindow.h"
#include "Core/Camera.h"

ProceduralTerrain::ProceduralTerrain(Camera* camera, std::string materialName) : VisualObject(materialName), mCamera(camera)
{
    mQuadTree = new QuadTree();

//    generateChunk(glm::vec2(0,0));
//    generateChunk(glm::vec2(0,0));
/*
    for(int j = 0; j < 3; j++)
        for(int i = 0; i < 3; i++) {
                TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*i,mChunkSize*j), BiomeType::Desert, mChunkSize, mChunkComplexity, materialName);
                mChunks.push_back(std::pair(glm::vec2(i,j) , chunk));
    }
    for(int j = 3; j < 6; j++)
        for(int i = 0; i < 3; i++) {
                TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*i,mChunkSize*j), BiomeType::Mountains, mChunkSize, mChunkComplexity, materialName);
                mChunks.push_back(std::pair(glm::vec2(i,j) , chunk));
    }
    for(int j = 0; j < 3; j++)
        for(int i = 3; i < 6; i++) {
                TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*i,mChunkSize*j), BiomeType::Hills, mChunkSize, mChunkComplexity, materialName);
                mChunks.push_back(std::pair(glm::vec2(i,j) , chunk));
    }

*/

    //for(int j = 0; j < 6; j++)
    //    for(int i = 0; i < 6; i++) {
    //            BiomeType biomeType = BiomeType(std::rand() % 3);
    //            TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*i,mChunkSize*j), biomeType, mChunkSize, mChunkComplexity, materialName);
    //            mChunks.push_back(chunk);
    //}

//    TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*0,mChunkSize*0), BiomeType::Hills, mChunkSize, mChunkComplexity, materialName);
//    mChunks.push_back(chunk);
//
//    chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*1,mChunkSize*0), BiomeType::Desert, mChunkSize, mChunkComplexity, materialName);
//    mChunks.push_back(chunk);
//
//    chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*0,mChunkSize*1), BiomeType::Mountains, mChunkSize, mChunkComplexity, materialName);
//    mChunks.push_back(chunk);

    //chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*1,mChunkSize*1), BiomeType::Hills, mChunkSize, mChunkComplexity, materialName);
    //mChunks.push_back(chunk);
}

ProceduralTerrain::~ProceduralTerrain()
{
    mChunks.clear();
}

void ProceduralTerrain::init()
{
    loadChunksWithinRadius();

    for(int i = 0; i < mChunks.size(); i++){
        mChunks[i].second->init();
    }
}

void ProceduralTerrain::draw()
{
    loadChunksWithinRadius();


    for(int i = 0; i < mChunks.size(); i++){
        mChunks[i].second->draw();
    }
}

void ProceduralTerrain::loadChunksWithinRadius()
{
    //Should only be called when player leaves a chunk
    //Get camera position
    if(!mCamera){
        std::cout << "Error! Camera does not exist when loading procedual chunks within radius.\n";
        return;
    }

    glm::vec2 camPos = mCamera->getPos();

    if(camPos == mCamPosOld && !mFirstLoad)
        return;

    //Create / check all chunks within radius if they need to be created or not

    float minXCoord = posToCoords(camPos.x - mRenderDistance) - 1;
    float minYCoord = posToCoords(camPos.y - mRenderDistance) - 1;
    float maxXCoord = posToCoords(camPos.x + mRenderDistance) + 1;
    float maxYCoord = posToCoords(camPos.y + mRenderDistance) + 1;

    for(int j = minYCoord; j <= maxYCoord; j++){
        for(int i = minXCoord; i <= maxXCoord; i++)
        {
            //glm::vec2 checkLocation;
            glm::vec2 chunkPos(i*mChunkSize,j*mChunkSize);
            glm::vec2 chunkCoords(i,j);

            //Check the cam position up towards the chunk position
            float distance = glm::length(chunkPos - camPos);
            if(distance <= mRenderDistance){
                //Chunk should be visible
                if(!chunkExistsAtCoords(chunkCoords)){
                    // Generate chunk if chunk at coords doesn't exist
                    TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed,
                                                                   glm::vec2(chunkCoords.x * mChunkSize, chunkCoords.y * mChunkSize),
                                                                   BiomeType::Hills,
                                                                   mChunkSize, mChunkComplexity,
                                                                   mMaterialName);
                    mChunks.push_back(std::pair(chunkCoords , chunk));
                    chunk->init();
                }
                else{
                    // do nothing
                }
            }
            else{
                //Chunk should not be visible
                if(chunkExistsAtCoords(chunkCoords))
                {
                    //Delete if chunk exists
                    for(int k = 0; k < mChunks.size(); k++){
                        if (mChunks[k].first == chunkCoords){
                            delete mChunks[k].second;
                            mChunks.erase(mChunks.begin() + k);
                            break;
                        }
                    }
                }
                else{
                    //Do nothing
                }
            }
        }
    }
    //Load chunks within radius

}

glm::vec2 ProceduralTerrain::posToCoords(glm::vec2 pos)
{
    float offset = mChunkSize/2;
    return glm::vec2( (int)((pos.x - offset) / mChunkSize)
                     ,(int)((pos.y - offset) / mChunkSize) );
}

float ProceduralTerrain::posToCoords(float pos)
{
    float offset = mChunkSize/2;
    return (int)((pos - offset) / mChunkSize);
}

bool ProceduralTerrain::chunkExistsAtCoords(glm::vec2 coords)
{
    for(int i = 0; i < mChunks.size(); i++){
        if (mChunks[i].first == coords)
            return true;
    }
    return false;
}

