#include "cube.h"

Cube::Cube()
{
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 0, 0, 1,  0,0));
    mVertices.push_back(Vertex(0.5,-0.5,-0.5    , 0, 0, 1,  1,0));
    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 0, 1,  0,1));

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 0, 1, 0,  0,0));
    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 1, 0,  0,1));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 0, 1, 0,  1,0));

    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 1, 0, 0,   0,1));
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 1, 0, 0,  0,0));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 1, 0, 0,  1,0));

    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 0, 0, 1,   0,1));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 0, 0, 1,  1,0));
    mVertices.push_back(Vertex(-0.5,0.5,-1.5   , 0, 0, 1,   0,0));

    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 0, 1, 0,   1,0));
    mVertices.push_back(Vertex(-0.5,0.5,-1.5   , 0, 1, 0,   0,0));
    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 0, 1, 0,   0,1));

    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 1, 0, 0,   1,0));
    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 1, 0, 0,   0,1));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 1, 0, 0,   0,0));

    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 0, 1,  0,1));
    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 0, 0, 1,   1,0));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 0, 0, 1,   0,0));

    mVertices.push_back(Vertex(0.5,-0.5,-0.5    , 0, 1, 0,  1,0));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 0, 1, 0,   0,0));
    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 1, 0,  0,1));

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 1, 0, 0,  0,0));
    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 1, 0, 0,   0,1));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 1, 0, 0,   1,0));

    mVertices.push_back(Vertex(0.5,-0.5,-0.5    , 0, 0, 1,  1,0));
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 0, 0, 1,  0,0));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 0, 0, 1,   0,1));

    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 0, 1, 0,  1,0));
    mVertices.push_back(Vertex(-0.5,0.5,-1.5   , 0, 1, 0,   0,0));
    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 0, 1, 0,   0,1));

    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 1, 0, 0,  0,1));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 1, 0, 0,  1,0));
    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 1, 0, 0,   0,0));

    calculateNormals(&mVertices, false);
}

Cube::~Cube()
{

}

void Cube::init()
{

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    scale(.5f,.5f,.5f);

}

void Cube::draw()                       // Almost the same as the draw function in xyz class, added translate, scale and rotate possibilities
{
    glBindVertexArray( mVAO );

    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, (GLfloat*)mTransformationMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
