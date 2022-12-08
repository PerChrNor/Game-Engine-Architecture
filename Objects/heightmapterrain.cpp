#include "heightmapterrain.h"

HeightMapTerrain::HeightMapTerrain(Texture *map, float hSpace, float vSpace) : mTexture{map}, mHSpace{hSpace}, mVSpace{vSpace}
{
    mWidth = mTexture->mColumns;
    mDepth = mTexture->mRows;

    generateTerrain();
}

HeightMapTerrain::~HeightMapTerrain()
{

}

void HeightMapTerrain::generateTerrain()
{
    Vector3D n{0.f, 1.f, 0.f};

    float width = (mWidth-1) * mHSpace;
    float vertXstart = -width / 2.f;
    float vertZstart = width / 2.f;
    for (unsigned int d = 0; d < mDepth; ++d)
    {
        for (unsigned int w = 0; w < mWidth; ++w)
        {
            float heighFromBitmap = mTexture->getHeightMap(w + d*mDepth) * mHSpace / 10.f - 5.f;
            heighFromBitmap = heighFromBitmap - 12.f;
            mVertices.emplace_back(Vector3D{vertXstart +(w*mHSpace), heighFromBitmap,
                                         vertZstart - (d*mHSpace)}, n, w / (mWidth-1.f), d / (mDepth-1.f));
        }
    }
    for (unsigned int d = 0; d < mDepth-1; ++d)
    {
        for (unsigned int w = 0; w < mWidth-1; ++w)
        {
            mIndices.emplace_back(w + d * mWidth);
            mIndices.emplace_back(w + d * mWidth + mWidth + 1);
            mIndices.emplace_back(w + d * mWidth + mWidth);
            mIndices.emplace_back(w + d * mWidth);
            mIndices.emplace_back(w + d * mWidth + 1);
            mIndices.emplace_back(w + d * mWidth + mWidth +1);
        }
    }

    mTransformationMatrix.setToIdentity();
    calculateNormals(&mVertices, false);
    qDebug() << "Terrain generated | Total Width:" << width << "| Size:" << mWidth << ", " << mDepth << "| VertexXStart:" << vertXstart << "| VertexZStart" << vertZstart;
}

void HeightMapTerrain::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof (GLuint), mIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void HeightMapTerrain::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, (GLfloat*)mTransformationMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

