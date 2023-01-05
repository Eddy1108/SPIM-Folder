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

    //Generate water
    mWater = new TerrainWater(0, mPos, mChunkSize, "materialwater");
}

TerrainBaseChunk::~TerrainBaseChunk()
{
    delete mWater;
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

    // -- Extreme Mountain Noise --
    // Side note: This is used to generate giant mountains in certain spots to create more variation between each mountains highest peak
    // Create and configure FastNoise object
    if(!mNoisePeaksAndValleys) {
        mNoisePeaksAndValleys = new FastNoiseLite(mSeed);
        noisePeakAndValleysTransformations();
        mNoisePeakAndValleysData = getNoiseData(mNoisePeaksAndValleys, noiseCoords,
                                             mChunkComplexity, mChunkComplexity);
    }

    // -- Height Offset Noise --
    // Create and configure FastNoise object
    if(!mNoiseHeightOffset) {
        mNoiseHeightOffset = new FastNoiseLite(mSeed);
        noiseHeightOffsetTransformation();
        mNoiseHeightOffsetData = getNoiseData(mNoiseHeightOffset, noiseCoords,
                                              mChunkComplexity, mChunkComplexity);
    }

    // -- Height Offset Noise --
    // Create and configure FastNoise object
    if(!mNoiseRivers) {
        mNoiseRivers = new FastNoiseLite(mSeed);
        noiseRiversTransformations();
        mNoiseRiversData = getNoiseData(mNoiseRivers, noiseCoords,
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
        mContinentalIntensity = 10;
        break;
    }

}

void TerrainBaseChunk::noisePeakAndValleysTransformations()
{
    mNoisePeaksAndValleys->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    mNoisePeaksAndValleys->SetFractalType(FastNoiseLite::FractalType_FBm);
    mNoisePeaksAndValleys->SetFrequency(0.002f);
    mNoisePeaksAndValleys->SetFractalOctaves(1);
    mNoisePeaksAndValleys->SetFractalLacunarity(0);
    mNoisePeaksAndValleys->SetFractalGain(0);
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

void TerrainBaseChunk::noiseRiversTransformations()
{
    mNoiseRivers->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    mNoiseRivers->SetFrequency(0.003f);
    mNoiseRivers->SetFractalType(FastNoiseLite::FractalType_Ridged);
    mNoiseRivers->SetFractalOctaves(1);
    mNoiseRivers->SetFractalLacunarity(0);
    mNoiseRivers->SetFractalGain(0);
    mRiversIntensity = 9;
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
    //...

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
            r = (z >= -1) ? (1-(z/(mContinentalIntensity))) : 0;
            g = (z >= -1) ? 0.5f : 0;
            //g = z+1;
            b = (z < -1) ? ((mContinentalIntensity/2+z)/(mContinentalIntensity)) : 0;

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
    float mountainPeaksRatio = (mNoisePeakAndValleysData[dataIndex] - mPeaksAndValleysFrequencyReduction) / (mPeaksAndValleysMaxHeight - mPeaksAndValleysFrequencyReduction);
    float valMountainPeaks = (mountainPeaksRatio >= 0.0f) ? (mountainPeaksRatio*mPeaksAndValleysIntensity + 1) : 1;
    float valHeightOffset = mNoiseHeightOffsetData[dataIndex] * mNoiseHeighOffsetIntensity;
    float riversRatio = (mNoiseRiversData[dataIndex] - mRiversFrequencyReduction) / (1 - mRiversFrequencyReduction);
    float valRivers = (mNoiseRiversData[dataIndex] >= mRiversFrequencyReduction) ? ((-riversRatio) * mRiversIntensity) : 0;

    // ---- Add together ----
    float z = 0;
    ///Create continental terrain (base noise)
    //z = (valContinental + 10) * valMountainPeaks - 10;
    z = valContinental;

    ///Slightly higher mountains at certain areas
    z += valMountainPeaks * valHeightOffset * 5 + valContinental * valMountainPeaks * 0.2f;

    //z += valHeightOffset;
    //z += valRivers;
    ///Should be done last
    z += mHeightOffset;

    //if(valRivers < 0)
    //   z = valRivers;

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

    //Init Water
    mWater->init();
}

void TerrainBaseChunk::draw()
{
    mMaterial->UpdateUniforms(&mMatrix);

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //Draw water
    mWater->draw();
}
