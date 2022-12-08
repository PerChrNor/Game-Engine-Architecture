#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include "texture.h"
#include "constants.h"

Texture::Texture() : QOpenGLFunctions_4_1_Core()
{
    initializeOpenGLFunctions();
    //small dummy texture
    for (int i=0; i<16; i++)
        pixels[i] = 0;
    pixels[0] = 255;
    pixels[4] = 255;
    pixels[8] = 255;
    pixels[9] = 255;
    pixels[10] = 255;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);
    qDebug() << "Texture::Texture() id = " << mId;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 reinterpret_cast<const GLvoid*>(pixels));
}

/**
 \brief Texture::Texture() Read a bitmap file and create a texture with standard parameters
 \param filename The name of the bmp file containing a texture
 First one 2D texture is generated from
 - glGenTextures()
 Then the OpenGL functions
 - glBindTexture()
 - glTexParameteri()
 - glTexImage2D()
 are used. The texture can be retrieved later by using the function id()
 */
Texture::Texture(const std::string& filename, bool cubeMap): QOpenGLFunctions_4_1_Core()
{
    textureFilename = filename;
    initializeOpenGLFunctions();
    if(cubeMap)
    {
        readCubeMap();
        setCubemapTexture();
    }
    else
    {
        readBitmap(filename);       //reads the BMP into memory
        setTexture();               //set texture up for OpenGL
    }
}

/**
    \brief Texture::id() Return the id of a previously generated texture object
    \return The id of a previously generated texture object
 */
GLuint Texture::id() const
{
    return mId;
}

float Texture::getHeightMap(int i)
{
    if (i > mColumns * mRows || i < 0)
        return 0;
    else
    {
        int r = mBitmap[i*3];
        int g = mBitmap[i*3+1];
        int b = mBitmap[i*3+2];

        float height = (r+g+b)/3.f;

        return  height;
    }
}

void Texture::readBitmap(const std::string &filename)
{
    OBITMAPFILEHEADER bmFileHeader;
    OBITMAPINFOHEADER bmInfoHeader;

    std::string fileWithPath = AssetFilePath + "Textures/" + filename;

    std::ifstream file;
    file.open (fileWithPath.c_str(), std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char *) &bmFileHeader, 14);

        file.read((char *) &bmInfoHeader, sizeof(OBITMAPINFOHEADER));
        mColumns = bmInfoHeader.biWidth;
        mRows = bmInfoHeader.biHeight;
        mnByte = bmInfoHeader.biBitCount / 8;

        mBitmap = new unsigned char[mColumns * mRows * mnByte];
        file.read((char *) mBitmap, mColumns * mRows * mnByte);
        file.close();
    }
    else
    {
        qDebug() << "Can not read " << QString(fileWithPath.c_str());
    }
    unsigned char tmp;
    // switch red and blue
    for (int k = 0; k < mColumns * mRows * mnByte; k += mnByte) {
        tmp = mBitmap[k];
        mBitmap[k] = mBitmap[k + 2];
        mBitmap[k + 2] = tmp;
    }
}

void Texture::readCubeMap()
{
    std::string justName;
    std::stringstream sStream;
    sStream << textureFilename;
    std::getline(sStream, justName, '.');   //deleting .bmp
    justName.pop_back();    //removing 1
    for(int i{0}; i< 6; i++)
    {
        std::string temp = "../CubeMaps/" +justName + std::to_string(i+1) + ".bmp";   //adding Cubemap path and 1 - 6 to filename
        readBitmap(temp);
        mCubemap[i] = mBitmap;
    }
}

void Texture::setTexture()
{
    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);
    qDebug() << "Texture::Texture() id = " << mId;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                mColumns,
                mRows,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                mBitmap);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setCubemapTexture()
{
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if(!mAlphaUsed)  //no alpha in this bmp
        for(int i{0}; i< 6; i++)
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, mColumns, mRows, 0, GL_BGR, GL_UNSIGNED_BYTE,  mCubemap[i]);
        }

    else    //alpha is present, so we set up an alpha channel
        qDebug() << "Skybox with alpha probably make no sense!?";

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    qDebug() << "Cubemap Texture" << textureFilename.c_str() << "successfully read | id = "
             << mTextureId << "| bytes pr pixel:" << mBytesPrPixel << "| using alpha:" << mAlphaUsed
             << "| w:" << mColumns << "|h:" << mRows;
}
