#include "terrainbasechunk.h"
#include "Noise/FastNoiseLite.h"



TerrainBaseChunk::TerrainBaseChunk(const int seed,
                                   glm::vec2 coords,
                                   BiomeType biomeType,
                                   const float chunkSize,
                                   const unsigned int chunkComplexity,
                                   std::string materialName )
    : VisualObject(materialName), mCoords(coords), mSeed(seed), mBiomeType(biomeType), mChunkSize(chunkSize), mChunkComplexity(chunkComplexity)
{
    generateFastNoise();
    generateChunk(coords);
}

void TerrainBaseChunk::generateFastNoise()
{
    // ---- Get coordinates ----
    // These coords are used when generating all noises
    glm::vec2 noiseCoords = glm::vec2( (mCoords.x / mChunkSize) * (mChunkComplexity -1),
                           (mCoords.y / mChunkSize) * (mChunkComplexity-1) );

    std::cout << "noiseCoords: " << noiseCoords.x << ", "<< noiseCoords.y << "\n";

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
    //if(!mNoiseHeightOffset) {
    //    mNoiseHeightOffset = new FastNoiseLite(mSeed);
    //    noiseContinentalnessTransformation();
    //mNoiseHeightOffsetData = getNoiseData(mNoiseHeightOffset, noiseCoords, mChunkComplexity, mChunkComplexity);

    //}
}

void TerrainBaseChunk::noiseContinentalnessTransformation()
{
    // Noise Settings
    switch(mBiomeType){
    case Mountains:
        mNoiseContinental->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        mNoiseContinental->SetFractalType(FastNoiseLite::FractalType_Ridged);
        mNoiseContinental->SetFrequency(0.005f);
        mNoiseContinental->SetFractalOctaves(8);
        mNoiseContinental->SetFractalLacunarity(2);
        mHeightIntensity = 4;
        break;

    case Desert:
        mNoiseContinental->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        mNoiseContinental->SetFractalType(FastNoiseLite::FractalType_Ridged);
        mNoiseContinental->SetFrequency(0.006f);
        mNoiseContinental->SetFractalOctaves(8);
        mNoiseContinental->SetFractalLacunarity(2);
        mNoiseContinental->SetFractalGain(0.0f);
        mHeightIntensity = 1.f;
        break;

    case Hills:
        mNoiseContinental->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        mNoiseContinental->SetFractalType(FastNoiseLite::FractalType_FBm);
        mNoiseContinental->SetFrequency(0.01f);
        mNoiseContinental->SetFractalOctaves(4);
        mNoiseContinental->SetFractalLacunarity(2);
        mNoiseContinental->SetFractalGain(0.4f);
        mHeightIntensity = 2;
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

    //Gets noise data for the chunk
    //glm::vec2 noiseCoords = glm::vec2( (coords.x / mChunkSize) * (mChunkComplexity -1),
    //                       (coords.y / mChunkSize) * (mChunkComplexity-1) );
    //std::vector<float> noiseData = generateNoiseData(noiseCoords, mChunkComplexity, mChunkComplexity);

    // ---- Create Vertices ----
    float x{0},y{0},z{0}, r{0},g{0.5f},b{0}, u{0},v{0};
    int indicesOffset = mVertices.size();

    //Normal / Color
    //r = 1;
    //g = 1;
    //b = 1;

    float delta = mChunkSize/(mChunkComplexity-1);
    for(int j = 0; j < mChunkComplexity; j++) {
        //XYZ - Y
        y = coords.y + j * delta;
        //UV - V Currently expected that each chunk has their own texture
        v = j/mChunkComplexity;

        for(int i = 0; i < mChunkComplexity; i++)
        {
            //XYZ - XZ
            x = coords.x + i * delta;
            // Get Z height from NoiseData
            z = getHeight(i, j);

            //UV - U - Currently expected that each chunk has their own texture
            u = i/mChunkComplexity;

            //debug colors
            r = z/2;
            b = -z/2;

            // Create Vertex
            mVertices.push_back(Vertex{ x, y, z, r, g, b, u,v });
            //std::cout << "x: " << x << ", y: " << y << ", z: " << z << "\n";
        }
    }

    // ---- Create indices ----
    for(int j = 0; j < mChunkComplexity-1; j++){
        for(int i = 0; i < mChunkComplexity-1; i++){
            //First triangle
            mIndices.push_back((i) + (j) * mChunkComplexity + indicesOffset);
            mIndices.push_back((i+1) + (j) * mChunkComplexity + indicesOffset);
            mIndices.push_back((i) + (j+1) * mChunkComplexity + indicesOffset);
            //Second triangle
            mIndices.push_back((i) + (j+1) * mChunkComplexity + indicesOffset);
            mIndices.push_back((i+1) + (j) * mChunkComplexity + indicesOffset);
            mIndices.push_back((i+1) + (j+1) * mChunkComplexity + indicesOffset);

        }
    }

    //for(int i = 0; i < mIndices.size(); i++){
    //    std::cout << mIndices[i] << ", ";
    //}

    //std::cout << "vertices size: " << mVertices.size() << "\n";
    //std::cout << "indices size: " << mIndices.size() << "\n";
}


float TerrainBaseChunk::getHeight(int i, int j)
{

    // ---- Gather noise data ----
    // ---- Add together ----
    float z = mNoiseContinentalData[i + j * mChunkComplexity];
    z *= mHeightIntensity;
    ///Should be done last
    z += mHeightOffset;

    return z;
}

std::vector<float> TerrainBaseChunk::getNoiseData(FastNoiseLite* fastNoise, glm::vec2 coords, int width, int height)
{
    // Gather noise data
    std::vector<float> noiseData(width * height);
    int index = 0;


    std::cout << "noiseData: ";
    for (int y = (int)coords.y; y < (int)coords.y + height; y++)
    {
        for (int x = coords.x; x < coords.x + width; x++)
        {
            noiseData[index++] = fastNoise->GetNoise((float)x, (float)y);
            std::cout << noiseData[index - 1] << ", ";
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
