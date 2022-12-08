#include "interactiveobject.h"
#include "Math/vector2d.h"

InteractiveObject::~InteractiveObject(){}

void InteractiveObject::init()
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
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);  

   glBindVertexArray(0);
}

void InteractiveObject::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, (GLfloat*)mTransformationMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void InteractiveObject::move(float x, float y, float z)
{
    mTransformationMatrix.translate(x, y, z);
}

void InteractiveObject::scale(float x, float y, float z)
{
    mTransformationMatrix.scale(x, y, z);
}

void InteractiveObject::rotate(int deg, float x, float y, float z)
{
    mTransformationMatrix.rotate(deg, x, y, z);
}

void InteractiveObject::barycentricMove(float x, float y, float z, VisualObject* ground)
{
    if (y==0)
    {
        mTransformationMatrix.translate(x, 0, 0);
        mTransformationMatrix.translate(0, 0, z);

        QVector3D bary;

        for (size_t i=0; i<ground->mIndices.size(); i++)
        {

            QVector3D p1 = ground->mVertices.at(ground->mIndices.at(i)).getXYZ();
            QVector3D p2 = ground->mVertices.at(ground->mIndices.at(++i)).getXYZ();
            QVector3D p3 = ground->mVertices.at(ground->mIndices.at(++i)).getXYZ();
            bary = barycentricCoords(QVector2D(p1.x(), p1.z()),
                                     QVector2D(p2.x(), p2.z()),
                                     QVector2D(p3.x(), p3.z()),
                                     QVector2D(mTransformationMatrix.column(3).x(), mTransformationMatrix.column(3).z()));

            if (bary.x() >=0 && bary.y() >=0 && bary.z() >=0)
            {
                QVector3D translation = {(0), (p1.y() * bary.x()) + (p2.y() * bary.y()) + (p3.y() * bary.z()), (0)};
                mTransformationMatrix.translate(0,translation.y()- mTransformationMatrix.column(3).y() + 0.5, 0);
                break;
            }
        }
    }
    else
    {
        mTransformationMatrix.translate(0,y,0);
    }
}


