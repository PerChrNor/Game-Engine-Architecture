#include "visualobject.h"
#include <QVector3D>

void VisualObject::getTriangles()
{
    unsigned int index{0};
    Vertex vertex1;
    Vertex vertex2;
    Vertex vertex3;

    while (index < mVertices.size())
    {
        vertex1 = mVertices.at(index++);
        vertex2 = mVertices.at(index++);
        vertex3 = mVertices.at(index++);

        triangles.push_back(TriangleStruct(vertex1, vertex2, vertex3));
    }
}

QVector3D VisualObject::barycentricCoords(const QVector2D &p1, const QVector2D &p2, const QVector2D &p3, const QVector2D &position)
{
    QVector2D p12 = p2-p1;
    QVector2D p13 = p3-p1;

    QVector3D n = {0,0,(p13.x()*p12.y())-(p13.y()*p12.x())};
    float areal_123 = n.length();

    QVector3D baryc{0,0,0};
    QVector2D p = p2 - position;
    QVector2D q = p3 - position;
    n = {0,0,(q.x()*p.y())-(q.y()*p.x())};
    baryc.setX(n.z() / areal_123);

    p = p3 - position;
    q = p1 - position;
    n = n = {0,0,(q.x()*p.y())-(q.y()*p.x())};
    baryc.setY(n.z() / areal_123);

    p = p1 - position;
    q = p2 - position;
    n = n = {0,0,(q.x()*p.y())-(q.y()*p.x())};
    baryc.setZ(n.z() / areal_123);

    return baryc;
}

void VisualObject::drawNormals(GLint matrixUniform)
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, mTransformationMatrix.constData());   //move to shader???
    if(mIndiceCount > 0)
        glDrawElements(GL_TRIANGLES, mIndiceCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    glBindVertexArray(0);
}

void VisualObject::readFile(std::string filename)
{
    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n;
        inn >> n;

        mVertices.reserve(n);
        for (int i=0; i<n/3; i++)
        {
            for (int j=0; j<3; ++j)
            {
                static float x, y, z;

                static float UVCoord{0};

                UVCoord = j;
                UVCoord /= 3;

                inn >> x; inn >> y; inn >> z;

                y *= -1;
                y += 2;

                mVertices.push_back(Vertex(x,y,z,0,0,0,UVCoord, UVCoord));
            }
        }
        inn.close();
    }
    getTriangles();
}

void VisualObject::writeFile(std::string filename)
{
    std::ofstream fout;
    fout.open(filename);

    if(fout.is_open())
    {
        int n = mVertices.size();
        Vertex vertex;
        fout << n << std::endl;

        for (int i{0}; i<n; ++i)
        {
            vertex = mVertices.at(i);
            fout << vertex << std::endl;
        }
        if (mIndices.size() > 0)
        {
            n = mIndices.size();
            fout << n << std::endl;

            int index{0};
            for(int i{0}; i<n; ++i)
            {
                index = mIndices.at(i);
                fout << index << " ";
            }
        }
        fout.close();
    }
}

void VisualObject::calculateNormals(std::vector<Vertex>* vertices, bool smooth)
{
    int it = 0;
    for (size_t i{0}; i < mVertices.size()/3; i++)
    {
        Vertex* V1 = &vertices->at(it++);
        Vertex* V2 = &vertices->at(it++);
        Vertex* V3 = &vertices->at(it++);

        QVector3D vec12 = (QVector3D(V2->getX(), V2->getY(), V2->getZ()) - QVector3D(V1->getX(), V1->getY(), V1->getZ())).normalized();
        QVector3D vec13 = (QVector3D(V3->getX(), V3->getY(), V3->getZ()) - QVector3D(V1->getX(), V1->getY(), V1->getZ())).normalized();

        QVector3D triangleNormal = QVector3D::crossProduct(vec13, vec12);

        V1->set_rgb(triangleNormal.x(), triangleNormal.y(), triangleNormal.z());
        V2->set_rgb(triangleNormal.x(), triangleNormal.y(), triangleNormal.z());
        V3->set_rgb(triangleNormal.x(), triangleNormal.y(), triangleNormal.z());
    }

    if(smooth)
    {
        for (auto x : *vertices)
        {
            importedVertexNorms.push_back(QVector3D(x.getR(), x.getG(), x.getB()));
            vertexPos.push_back(QVector3D(x.getX(), x.getY(), x.getZ()));
        }

        for (size_t i{0}; i < importedVertexNorms.size(); i++) smoothedVertexNorms.push_back(QVector3D(0,0,0));

        std::vector<bool> visited;
        for (size_t i{0}; i < importedVertexNorms.size(); i++) visited.push_back(false);

        std::vector<int> visitedIndex;

        QVector3D sumNormal;

        for (unsigned int i{0}; i < importedVertexNorms.size(); i++)
        {
            visitedIndex.clear();
            sumNormal = QVector3D(0,0,0);

            if(!visited.at(i))
            {
                visited.at(i) = true;
                sumNormal += importedVertexNorms.at(i);
                visitedIndex.push_back(i);

                for (unsigned int j{i+1}; j < importedVertexNorms.size(); j++)
                {
                    if(!visited.at(j) && vertexPos.at(j) == vertexPos.at(i))
                    {
                        sumNormal += importedVertexNorms.at(j);
                        visitedIndex.push_back(j);
                        visited.at(j) = true;
                    }
                }


                for (auto x: visitedIndex)
                {
                    smoothedVertexNorms.at(x) = sumNormal.normalized();
                }
            }
        }

        for (size_t i{0}; i < vertices->size(); i++)
        {
            vertices->at(i).set_rgb(smoothedVertexNorms.at(i).x(),
                                    smoothedVertexNorms.at(i).y(),
                                    smoothedVertexNorms.at(i).z());
        }
    }
}

void VisualObject::setBoundingBox(std::vector<Vertex>& positions)
{
    boxMinX = positions.at(0).getX(); boxMinY = positions.at(0).getY(); boxMinZ = positions.at(0).getZ();
    boxMaxX = positions.at(0).getX(); boxMaxY = positions.at(0).getY(); boxMaxZ = positions.at(0).getZ();

    for (auto x : positions){
        if (x.getX() < boxMinX) boxMinX = x.getX();
        if (x.getX() > boxMaxX) boxMaxX = x.getX();

        if (x.getY() < boxMinY) boxMinY = x.getY();
        if (x.getY() > boxMaxY) boxMaxY = x.getY();

        if (x.getZ() > boxMinZ) boxMinZ = x.getZ();
        if (x.getZ() < boxMaxZ) boxMaxZ = x.getZ();
    }

    //construct box min/max
    boxMin.x = boxMinX; boxMin.y = boxMinY; boxMin.z = boxMaxZ;
    boxMax.x = boxMaxX; boxMax.y = boxMaxY; boxMax.z = boxMinZ;
}

void VisualObject::updateBoundingBox()
{
    boxMin.x = mTransformationMatrix.column(3).x() + boxMinX;
    boxMin.y = mTransformationMatrix.column(3).y() + boxMinY;
    boxMin.z = mTransformationMatrix.column(3).z() + boxMinZ;

    boxMax.x = mTransformationMatrix.column(3).x() + boxMaxX;
    boxMax.y = mTransformationMatrix.column(3).y() + boxMaxY;
    boxMax.z = mTransformationMatrix.column(3).z() + boxMaxZ;
}

bool VisualObject::CheckCollide(Vector3D Pmin, Vector3D Pmax)
{
    bool result;

    if(boxMax.x >= Pmin.x &&
       boxMin.x <= Pmax.x &&
       boxMax.y >= Pmin.y &&
       boxMin.y <= Pmax.y &&
       boxMax.z <= Pmin.z &&
       boxMin.z >= Pmax.z)
    {
        result = true;
    }

    else
    {
        result = false;
    }

   return result;
}
