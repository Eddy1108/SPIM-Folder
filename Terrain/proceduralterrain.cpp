#include "proceduralterrain.h"
#include "Noise/FastNoiseLite.h"
#include "terrainbasechunk.h"

#include "renderwindow.h"
#include "Core/Camera.h"

ProceduralTerrain::ProceduralTerrain(Camera* camera, std::string materialName) : VisualObject(materialName), mCamera(camera)
{
    mQuadTree = new QuadTree();


    // ---- Code used for testing ----

//    generateChunk(glm::vec2(0,0));

    //for(int j = 0; j < 6; j++)
    //    for(int i = 0; i < 6; i++) {
    //            BiomeType biomeType = BiomeType(std::rand() % 3);
    //            TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*i,mChunkSize*j), biomeType, mChunkSize, mChunkComplexity, materialName);
    //            mChunks.push_back(chunk);
    //}

    //TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*0,mChunkSize*0), BiomeType::Desert, mChunkSize, mChunkComplexity, materialName);
    //mChunks.push_back(std::make_pair(glm::vec2(0,0), chunk));
//
    //chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*1,mChunkSize*0), BiomeType::Desert, mChunkSize, mChunkComplexity, materialName);
    //mChunks.push_back(std::make_pair(glm::vec2(1,0), chunk));
//
    //chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*0,mChunkSize*1), BiomeType::Hills, mChunkSize, mChunkComplexity, materialName);
    //mChunks.push_back(std::make_pair(glm::vec2(0,1), chunk));
//
    //chunk = new TerrainBaseChunk(mSeed, glm::vec2(mChunkSize*1,mChunkSize*1), BiomeType::Mountains, mChunkSize, mChunkComplexity, materialName);
    //mChunks.push_back(std::make_pair(glm::vec2(1,1), chunk));
}

ProceduralTerrain::~ProceduralTerrain()
{
    mChunks.clear();
}

void ProceduralTerrain::init()
{
    loadChunksWithinRadius();

    for (auto it = mChunks.begin(); it != mChunks.end(); it++) {
        (*it).second->init();
    }
}

void ProceduralTerrain::draw()
{
    loadChunksWithinRadius();

    for (auto it = mChunks.begin(); it != mChunks.end(); it++) {
        (*it).second->draw();
    }
}

void ProceduralTerrain::loadChunksWithinRadius()
{
    //Should only update when the player moves
    //Get camera position
    if(!mCamera){
        std::cout << "Error! Camera does not exist when loading procedual chunks within radius.\n";
        return;
    }

    glm::vec2 camPos = mCamera->getPos();
    if(camPos == mCamPosOld && !mFirstLoad)
        return;

    // Find area of which chunks should be created within
    float minXCoord = posToCoords(camPos.x - mRenderDistance);
    float minYCoord = posToCoords(camPos.y - mRenderDistance);
    float maxXCoord = posToCoords(camPos.x + mRenderDistance);
    float maxYCoord = posToCoords(camPos.y + mRenderDistance);


    // ---- Delete Chunks ----
    for(auto it = mChunks.begin(); it != mChunks.end(); it++)
    {
        //Delete chunks if they're outside of radius.
        glm::vec2 chunkPos = (*it).second->getPos();

        float distance = glm::length(chunkPos - camPos);

        //Check if chunks are outside of renderDistance
        if(distance <= mRenderDistance) {

            //Check if chunk has correct LOD
            float LOD = (*it).second->getLOD();
            float a,c;

            if(LOD != 0)
                a = mRenderDistance / (mipMap - LOD + 2);
            else
                a = -0.1;

            c = mRenderDistance / (mipMap - LOD + 1);
            //Compare
            if(a < distance && distance <= c){
                //Chunk has correct LOD
            }
            else {
                //Delete chunk due to wrong LOD
                delete (*it).second;
                mChunks.erase((*it).first);
                break;
            }
        }
        else {
            //Delete chunk due to it being out of render distance
            delete (*it).second;
            mChunks.erase((*it).first);
            break;
        }
    }

    // ---- Generate Chunks ----
    for(int j = minYCoord; j <= maxYCoord; j++) {
        for(int i = minXCoord; i <= maxXCoord; i++)
        {
            //Get chunk Position and Coordinates
            glm::vec2 chunkPos(i*mChunkSize,j*mChunkSize);
            glm::vec2 chunkCoords(i,j);

            //Get the disance from camera's XY position to the chunk's position
            float distance = glm::length(chunkPos - camPos);
            //Checks if the chunk is outside of view distance, meaning that the chunk should not be loaded
            for(int LOD = 0; LOD <= mipMap; LOD++){
                if(distance <= mRenderDistance / (mipMap - LOD + 1)){
                    //Chunk should be visible
                    if(!chunkExistsAtCoords(chunkCoords)){
                        // Generate chunk if chunk at coords doesn't exist
                        generateChunk(chunkCoords, LOD);
                    }
                    break;
                }
            }
            //Temporary code
//            if(distance <= mRenderDistance){
//                //Chunk should be visible
//                if(!chunkExistsAtCoords(chunkCoords)){
//                    // Generate chunk if chunk at coords doesn't exist
//                    generateChunk(chunkCoords, 6);
//                }
//            }
        }
    }
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
    //Creates chunkID from Coords
    std::string chunkID = std::to_string(coords.x) + "," + std::to_string(coords.y);
    //Finds out if a chunk with given ID exists or not
    if(mChunks.find(chunkID) != mChunks.end())
        return true;
    else
        return false;
}

void ProceduralTerrain::generateChunk(glm::vec2 coords, unsigned int levelOfDetail)
{
    TerrainBaseChunk* chunk = new TerrainBaseChunk(mSeed,
                                                   glm::vec2(coords.x * mChunkSize, coords.y * mChunkSize),
                                                   BiomeType::Mountains,
                                                   mChunkSize, mChunkComplexity,
                                                   mMaterialName,
                                                   levelOfDetail);
    std::string chunkID = std::to_string(coords.x) + "," + std::to_string(coords.y);

    std::pair<std::string,TerrainBaseChunk*> newChunk (chunkID, chunk);
    mChunks.insert(newChunk);
    chunk->init();
}

