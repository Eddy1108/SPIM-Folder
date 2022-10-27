#include <iostream>
#include <fstream>
#include <string>

#include "texture.h"

Texture::Texture()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    //Or you can replace this with writing to QDebug or cout

    initializeOpenGLFunctions();

    //Make small dummy texture
    makeDummyTexture();
}

Texture::Texture(const std::string& filename) : QOpenGLFunctions_4_1_Core()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    textureFilename = filename;
    initializeOpenGLFunctions();
    bool success = readBitmap(filename);       //reads the BMP into memory
    if (success)
        setTexture();               //set texture up for OpenGL
}

GLuint Texture::id() const
{
    return mId;
}

unsigned char* Texture::getBitmap()
{
    return mBitmap;
}

bool Texture::readBitmap(const std::string& filename)
{
    OBITMAPFILEHEADER bmFileHeader;
    OBITMAPINFOHEADER bmInfoHeader;

    std::string fileWithPath = filename;

    std::ifstream file;
    file.open(fileWithPath.c_str(), std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char*)&bmFileHeader, 14);

        file.read((char*)&bmInfoHeader, sizeof(OBITMAPINFOHEADER));

        if (bmFileHeader.bfType != 19778)
        {
            std::cout << "ERROR: File is not a propper BMP file - no BM as first bytes" << std::endl;
            makeDummyTexture();
            return false;
        }

        mColumns = bmInfoHeader.biWidth;
        mRows = bmInfoHeader.biHeight;
        mBytesPrPixel = bmInfoHeader.biBitCount / 8;
        if (mBytesPrPixel == 4)
            mAlphaUsed = true;

        if (mBytesPrPixel < 3)    //we only support 24 or 32 bit images
        {
            std::cout << "ERROR: Image not 24 or 32 bit RBG or RBGA";
            makeDummyTexture();
            return false;
        }

        mBitmap = new unsigned char[mColumns * mRows * mBytesPrPixel];

        //check if image data is offset - most often not used...
        if (bmFileHeader.bfOffBits != 0)
            file.seekg(bmFileHeader.bfOffBits);
        else if (bmInfoHeader.biSize != 40) //try next trick if file is not a plain old BMP
        {
            int discard = bmInfoHeader.biSize - sizeof(OBITMAPINFOHEADER);
            char* temp = new char[discard];
            file.read(temp, discard);   //discard extra info if header is not old 40 byte header
            std::cout << "WARNING: InfoHeader is not 40 bytes, so image might not be correct" << std::endl;
            delete[] temp;
        }
        file.read((char*)mBitmap, mColumns * mRows * mBytesPrPixel);
        file.close();
        return true;
    }
    else
    {
        std::cout << "ERROR: Can not read " + fileWithPath << std::endl;
        makeDummyTexture();
    }
    return false;
}

//Set up texture for use in OpenGL
void Texture::setTexture()
{
    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);

    std::cout << "Texture " + textureFilename + " successfully read | id = " + std::to_string(mId) +
        "| bytes pr pixel: " + std::to_string(mBytesPrPixel) + " | using alpha:" + std::to_string(mAlphaUsed);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (mAlphaUsed == false)                     //no alpha in this bmp
        glTexImage2D(
            GL_TEXTURE_2D,
            0,                  //mipmap level
            GL_RGB,             //internal format - what format should OpenGL use
            mColumns,
            mRows,
            0,                  //always 0
            GL_BGR,             //format of data from texture file -  bmp uses BGR, not RGB
            GL_UNSIGNED_BYTE,   //size of each color channel
            mBitmap);           //pointer to texture in memory

    else                                //alpha is present, so we set up an alpha channel
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mColumns, mRows, 0, GL_BGRA, GL_UNSIGNED_BYTE, mBitmap);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::makeDummyTexture()
{
    //std::cout << "Making dummy texture";
    for (int i = 0; i < 16; i++)
        pixels[i] = 0;
    pixels[0] = 255;
    pixels[4] = 255;
    pixels[8] = 255;
    pixels[9] = 255;
    pixels[10] = 255;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);
    std::cout << "Texture id:" + std::to_string(mId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
        reinterpret_cast<const GLvoid*>(pixels));
}