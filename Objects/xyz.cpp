#include "xyz.h"

XYZ::XYZ()
{
    // x
    mVertices.push_back(Vertex{0, 0, 0, 1, 0, 0});
    mVertices.push_back(Vertex{3, 0, 0, 1, 0, 0});
    // y
    mVertices.push_back(Vertex{0, 0, 0, 0, 1, 0});
    mVertices.push_back(Vertex{0, 3, 0, 0, 1, 0});
    // z
    mVertices.push_back(Vertex{0, 0, 0, 0, 0, 1});
    mVertices.push_back(Vertex{0, 0, 3, 0, 0, 1});

    mGrid = new Grid_Area(20);

    mTransformationMatrix.setToIdentity();
}

XYZ::~XYZ()
{

}

void XYZ::init()
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

    mGrid->init();
    glBindVertexArray(0);
}

void XYZ::draw()
{
    glBindVertexArray( mVAO );

    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, (GLfloat*)mTransformationMatrix.constData());

    glDrawArrays(GL_LINES, 0, 6);

    mGrid->draw();
}
