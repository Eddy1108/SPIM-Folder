#include "terrainbasechunk.h"
#include "Noise/FastNoiseLite.h"



TerrainBaseChunk::TerrainBaseChunk(const int seed,
                                   glm::vec2 position,
                                   BiomeType biomeType,
                                   const float chunkSize,
                                   const unsigned int chunkComplexity,
                                   std::string materialName,
                                   const short unsigned int levelOfDetail)
    : VisualObject(materialName), mPos(position), mSeed(seed), mBiomeType(biomeType), mChunkSize(chunkSize), mChunkComplexity(chunkComplexity), mLevelOfDetail(levelOfDetail)
{

    generateFastNoise();
    generateChunk(position);

    mNoiseContinentalData.clear();
    mNoiseHeightOffsetData.clear();
}

void TerrainBaseChunk::generateFastNoise()
{
    // ---- Get coordinates ----
    // These coords are used when generating all noises
    glm::vec2 noiseCoords = glm::vec2( (mPos.x / mChunkSize) * (mChunkComplexity -1),
                           (mPos.y / mChunkSize) * (mChunkComplexity-1) );

    // -- Continental Noise --
    // Create and configure FastNoise object
    if(!mNoiseContinental) {
        mNoiseContinental = new FastNoiseLite(mSeed);
        noiseContinentalnessTransformation();
        mNoiseContinentalData = getNoiseData(mNoiseContinental, noiseCoords,
                                             mChunkComplexity, mChunkComplexity);
    }

    // -- Height Offset Noise --
    // Create and configure FastNoise object
    if(!mNoiseHeightOffset) {
        mNoiseHeightOffset = new FastNoiseLite(mSeed);
        noiseContinentalnessTransformation();
        mNoiseHeightOffsetData = getNoiseData(mNoiseHeightOffset, noiseCoords,
                                              mChunkComplexity, mChunkComplexity);
    }
}

void TerrainBaseChunk::noiseContinentalnessTransformation()
{
    // Noise Settings
    switch(mBiomeType){
    case Mountains:
        mNoiseContinental->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        mNoiseContinental->SetFractalType(FastNoiseLite::FractalType_Ridged);
        //mNoiseContinental->SetFrequency(0.005f);
        mNoiseContinental->SetFrequency(0.002f);
        mNoiseContinental->SetFractalOctaves(8);
        mNoiseContinental->SetFractalLacunarity(2);
        mContinentalIntensity = 16;
        mHeightOffset = 3;
        mNoiseHeighOffsetIntensity = 1;
        break;

    case Desert:
        mNoiseContinental->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        mNoiseContinental->SetFractalType(FastNoiseLite::FractalType_Ridged);
        mNoiseContinental->SetFrequency(0.006f);
        mNoiseContinental->SetFractalOctaves(8);
        mNoiseContinental->SetFractalLacunarity(2);
        mNoiseContinental->SetFractalGain(0.0f);
        mContinentalIntensity = 1.f;
        break;

    case Hills:
        mNoiseContinental->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        mNoiseContinental->SetFractalType(FastNoiseLite::FractalType_FBm);
        mNoiseContinental->SetFrequency(0.005f);
        mNoiseContinental->SetFractalOctaves(4);
        mNoiseContinental->SetFractalLacunarity(2);
        mNoiseContinental->SetFractalGain(0.4f);
        mContinentalIntensity = 2;
        break;
    }

}

void TerrainBaseChunk::noiseHeightOffsetTransformation()
{
    mNoiseHeightOffset->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    mNoiseHeightOffset->SetFractalType(FastNoiseLite::FractalType_None);
    mNoiseHeightOffset->SetFrequency(0.002f);
    mNoiseHeightOffset->SetFractalOctaves(1);
    mNoiseHeightOffset->SetFractalLacunarity(0);
    mNoiseHeightOffset->SetFractalGain(0);
}


void TerrainBaseChunk::generateChunk(glm::vec2 coords)
{
    if(std::fmodf(coords.x/mChunkSize, 1) != 0 || std::fmodf( coords.y/mChunkSize, 1) != 0){
        std::cout << "Error! Chunk at coords: " << coords.x << ", " << coords.y << ", can not be generated due to coords given being incorrect. The coords should instead be divisible by mChunkSize: " << mChunkSize << " and return a whole number." << std::endl;
        return;
    }

    // ---- Create Vertices ----
    float x{0},y{0},z{0}, r{0},g{0.5f},b{0}, u{0},v{0};
    int indicesStartOffset = mVertices.size(); // Currently always 0
    float xOffset = -mChunkSize/2, yOffset = -mChunkSize/2;

    //Normal / Color


    float iteratorIncrement = std::pow(2, mLevelOfDetail);
    float delta = mChunkSize/(mChunkComplexity-1);
    for(int j = 0; j < mChunkComplexity; j += iteratorIncrement) {
        //XYZ - Y
        y = coords.y + j * delta;
        //UV - V Currently expected that each chunk has their own texture
        v = (float)j/mChunkComplexity;

        for(int i = 0; i < mChunkComplexity; i += iteratorIncrement)
        {
            //XYZ - XZ
            x = coords.x + i * delta;
            // Get Z height from NoiseData
            z = getHeight(i, j);

            //UV - U - Currently expected that each chunk has their own texture
            u = (float)i/mChunkComplexity;

            //<<debug colors>>
            r = z/(mContinentalIntensity);
            //g = z+1;
            b = -z/(mContinentalIntensity);

            // Create Vertex
            mVertices.push_back(Vertex{ x + xOffset, y + yOffset, z, r, g, b, u,v });
            //std::cout << "x: " << x << ", y: " << y << ", z: " << z << "\n";
        }
    }

    // ---- Create indices ----
    float indicesRowOffset = (mChunkComplexity - 1 ) / (iteratorIncrement) + 1;
    for(int j = 0; j < std::sqrt(mVertices.size())-1; j++){
        for(int i = 0; i < std::sqrt(mVertices.size())-1; i++){
            //First triangle
            mIndices.push_back((i) + (j) * indicesRowOffset + indicesStartOffset);
            mIndices.push_back((i+1) + (j) * indicesRowOffset + indicesStartOffset);
            mIndices.push_back((i) + (j+1) * indicesRowOffset + indicesStartOffset);
            //Second triangle
            mIndices.push_back((i) + (j+1) * indicesRowOffset + indicesStartOffset);
            mIndices.push_back((i+1) + (j) * indicesRowOffset + indicesStartOffset);
            mIndices.push_back((i+1) + (j+1) * indicesRowOffset + indicesStartOffset);

        }
    }
}


float TerrainBaseChunk::getHeight(int i, int j)
{
    int dataIndex = i + j * mChunkComplexity;
    // ---- Gather noise data ----
    float valContinental = mNoiseContinentalData[dataIndex] * mContinentalIntensity;
    float valHeightOffset = mNoiseHeightOffsetData[dataIndex] * mNoiseHeighOffsetIntensity;
    // ---- Add together ----
    float z = valContinental;

    //z += std::clamp(valHeightOffset, 0.0f, 1.f);
    z+= valHeightOffset;
    ///Should be done last
    z += mHeightOffset;

    return z;
}

glm::vec2 TerrainBaseChunk::getCoords()
{
    return mPos/mChunkSize;
}

glm::vec2 TerrainBaseChunk::getPos()
{
    return mPos;
}

int TerrainBaseChunk::getLOD()
{
    return mLevelOfDetail;
}

std::vector<float> TerrainBaseChunk::getNoiseData(FastNoiseLite* fastNoise, glm::vec2 coords, int width, int height)
{
    // Gather noise data
    std::vector<float> noiseData(width * height);
    int index = 0;


    //std::cout << "noiseData: ";
    for (int y = (int)coords.y; y < (int)coords.y + height; y++)
    {
        for (int x = coords.x; x < coords.x + width; x++)
        {
            noiseData[index++] = fastNoise->GetNoise((float)x, (float)y);
        }
    }

    return noiseData;
}

void TerrainBaseChunk::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,      //what buffer type
        mVertices.size() * sizeof(Vertex),			   //how big buffer do we need
        mVertices.data(),             //the actual vertices
        GL_STATIC_DRAW				 //should the buffer be updated on the GPU
    );

    //EBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);


    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
        3,                  // size / number of elements of data type
        GL_FLOAT,           // data type
        GL_FALSE,           // normalize data
        sizeof(Vertex),  // stride
        reinterpret_cast<GLvoid*>(0));          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same parameter list as above but attribute and offset is adjusted accoringly
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);	//release
}

void TerrainBaseChunk::draw()
{
    mMaterial->UpdateUniforms(&mMatrix);

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
