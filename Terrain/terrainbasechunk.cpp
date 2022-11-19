#include "terrainbasechunk.h"
#include "Noise/FastNoiseLite.h"


enum BiomeType
{
    Mountains,
    Desert
};

TerrainBaseChunk::TerrainBaseChunk(const int seed,
                                   glm::vec2 coords,
                                   const float chunkSize,
                                   const unsigned int chunkComplexity,
                                   std::string materialName )
    : VisualObject(materialName), mSeed(seed), mChunkSize(chunkSize), mChunkComplexity(chunkComplexity)
{
    generateChunk(coords);
}

void TerrainBaseChunk::generateChunk(glm::vec2 coords)
{
    if(std::fmodf(coords.x/mChunkSize, 1) != 0 || std::fmodf( coords.y/mChunkSize, 1) != 0){
        std::cout << "Error! Chunk at coords: " << coords.x << ", " << coords.y << ", can not be generated due to coords given being incorrect. The coords should instead be divisible by mChunkSize: " << mChunkSize << " and return a whole number." << std::endl;
        return;
    }

    //Gets noise data for the chunk
    glm::vec2 noiseCoords = glm::vec2( (coords.x / mChunkSize) * (mChunkComplexity -1),
                           (coords.y / mChunkSize) * (mChunkComplexity-1) );


    glm::vec2 noiseCoords1 = glm::vec2( coords.x,
                           (coords.y / mChunkSize) * (mChunkComplexity-1) );

    std::vector<float> noiseData = generateNoiseData(noiseCoords, mChunkComplexity, mChunkComplexity);

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
            z = noiseData[i + j * mChunkComplexity];

            //UV - U - Currently expected that each chunk has their own texture
            u = i/mChunkComplexity;

            //debug colors
            r = z;
            b = -z;

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


std::vector<float> TerrainBaseChunk::generateNoiseData(glm::vec2 coords, int width, int height)
{
    std::cout << "seed: " << mSeed << "\n";
    // Create and configure FastNoise object
    FastNoiseLite mNoise(mSeed);
    // Noise Settings
    mNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    mNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    mNoise.SetFrequency(0.005f);
    mNoise.SetFractalOctaves(8);
    mNoise.SetFractalLacunarity(2);
    //mNoise.SetFractalGain(0.f);

    // Gather noise data
    std::vector<float> noiseData(width * height);
    int index = 0;

    for (int y = (int)coords.y; y < (int)coords.y + height; y++)
    {
        for (int x = coords.x; x < coords.x + width; x++)
        {
            noiseData[index++] = mNoise.GetNoise((float)x, (float)y);
        }
    }

    return noiseData;
}

void TerrainBaseChunk::init()
{
    initializeOpenGLFunctions();

    //Grass texture from here: https://www.pinterest.com/pin/texture-png-seamless-tileable-grass--596867756834423269/
    //mTexture = new Texture("../SPIM-Folder/Assets/Texture/RealGrass.bmp");
    //mHeightmap = new Texture("../SPIM-Folder/Assets/Texture/EksamenHeightmap.bmp");

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
