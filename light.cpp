#include "light.h"

light::light()
{
    mVertices.push_back(Vertex(-0.25f, -0.25f,  0.25f,   0.8f, 0.8f, 0.3f,   0.f,  0.f));
    mVertices.push_back(Vertex(0.25f, -0.25f,  0.25f,   0.8f, 0.8f, 0.3f,   1.f,  0.f));
    mVertices.push_back(Vertex(0.0f, 0.25f,  0.0f,   0.8f, 0.8f, 0.3f,   0.5f,  0.5f));

    mVertices.push_back(Vertex(0.25f, -0.25f,  0.25f,   0.8f, 0.8f, 0.3f,   1.f,  0.f));
    mVertices.push_back(Vertex(0.0f, -0.25f,  -0.25f,   0.8f, 0.8f, 0.3f,   0.5f,  0.5f));
    mVertices.push_back(Vertex(0.0f, 0.25f,  0.0f,   0.8f, 0.8f, 0.3f,   0.5f,  0.5f));

    mVertices.push_back(Vertex(0.0f, -0.25f,  -0.25f,   0.8f, 0.8f, 0.3f,   0.5f,  0.5f));
    mVertices.push_back(Vertex(-0.25f, -0.25f,  0.25f,   0.8f, 0.8f, 0.3f,   0.f,  0.f));
    mVertices.push_back(Vertex(0.0f, 0.25f,  0.0f,   0.8f, 0.8f, 0.3f,   0.5f,  0.5f));

    mVertices.push_back(Vertex(-0.25f, -0.25f,  0.25f,   0.8f, 0.8f, 0.3f,   0.f,  0.f));
    mVertices.push_back(Vertex(0.0f, -0.25f,  -0.25f,   0.8f, 0.8f, 0.3f,   0.5f,  0.5f));
    mVertices.push_back(Vertex(0.25f, -0.25f,  0.25f,   0.8f, 0.8f, 0.3f,   1.f,  0.f));


    mTransformationMatrix.setToIdentity();
}

void light::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertices.size() * sizeof(GLuint), mVertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void light::draw()
{;
        glBindVertexArray( mVAO );
        glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, (GLfloat*)mTransformationMatrix.constData());
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

}
