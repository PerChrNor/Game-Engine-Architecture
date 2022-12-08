#include "grid_area.h"

Grid_Area::Grid_Area(int grids)
    : mNumberOfGrids(grids), mOuterGridPosition(0), mGridColor(0.3, 0.3, 0.3)
{
    float startPosition = -2.f;
    float endPosition = 2.f;

    for(int i = 0; i < mNumberOfGrids; i++) {
        int prefix = 1;
        if(mOuterGridPosition == 0.f) {
            endPosition = 0.f;
        } else {
            endPosition = 2.f;
        }
        for(int j = 0; j < 2; j++) {
            // along x-axis
            Vertex xVertexFrom;
            xVertexFrom.set_xyz(startPosition, 0, prefix * mOuterGridPosition);
            xVertexFrom.set_rgb(mGridColor);
            Vertex xVertexTo;
            xVertexTo.set_xyz(endPosition, 0, prefix * mOuterGridPosition);
            xVertexTo.set_rgb(mGridColor);
            mVertices.push_back(xVertexFrom);
            mVertices.push_back(xVertexTo);

            // along z-axis
            Vertex zVertexFrom;
            zVertexFrom.set_xyz(prefix * mOuterGridPosition, 0, startPosition);
            zVertexFrom.set_rgb(mGridColor);
            Vertex zVertexTo;
            zVertexTo.set_xyz(prefix * mOuterGridPosition, 0, endPosition);
            zVertexTo.set_rgb(mGridColor);
            mVertices.push_back(zVertexFrom);
            mVertices.push_back(zVertexTo);

            prefix = -1;
        }
        mOuterGridPosition += 0.1f;
    }

    mTransformationMatrix.setToIdentity();
}

Grid_Area::~Grid_Area() {

}

void Grid_Area::init()
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

    glBindVertexArray(0);
}

void Grid_Area::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mTransformationMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}
