#include "objmesh.h"

#include <sstream>
#include "resourcemanager.h"

objMesh::objMesh(std::string fileName)
{
    readFile(fileName);
    mTransformationMatrix.setToIdentity();
}

objMesh::objMesh(std::string cameraName, Vector3D cornerNear, Vector3D cornerFar)
{
    mMeshName = cameraName;
    makeFrustum(cornerNear, cornerFar);
    init();
}

objMesh::~objMesh()
{

}

void objMesh::init()
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

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof (GLuint), mIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

    mTransformationMatrix.setToIdentity();
    mTransformationMatrix.scale(10,10,10);

    setBoundingBox(mVertices);
}

void objMesh::readFile(std::string fileName)
{
    std::ifstream fileIn;
    fileIn.open(fileName, std::ifstream::in);

    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(fileName);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<Vector3D> tempVert;
    std::vector<Vector3D> tempNorm;
    std::vector<QVector2D> tempUVs;

    unsigned int temp_index = 0;

    while(std::getline(fileIn, oneLine))
    {
        std::stringstream sStream;
        sStream << oneLine;

        oneWord = "";
        sStream >> oneWord;

        if(oneWord == "#")
        {
            continue;
        }
        if(oneWord == "")
        {
            continue;
        }
        if(oneWord == "v")
        {
            Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            tempVert.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            QVector2D tempUV;
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));

            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            tempNorm.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;
                std::stringstream tempWord(oneWord);
                std::string segment;
                std::vector<std::string> segmentArray;
                while(std::getline(tempWord, segment, '/'))
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);
                if (segmentArray[1] != "")
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    uv = 0;
                }
                normal = std::stoi(segmentArray[2]);

                --index;
                --uv;
                --normal;

                if(uv > -1)
                {
                    Vertex tempVertices(tempVert[index], tempNorm[normal], tempUVs[uv].x(), tempUVs[uv].y());
                    mVertices.push_back(tempVertices);
                }
                else
                {
                    Vertex tempVertices(tempVert[index], tempNorm[normal], 0, 0);
                    mVertices.push_back(tempVertices);
                }
                mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    fileIn.close();
}

void objMesh::makeVertex()
{
    while(true)
    {
        if(oneLine[OBJi] != '/' && oneLine[OBJi] != ' ' ) {oneWord += oneLine[OBJi]; OBJi++;}
        else {indexPos = std::stoi(oneWord); oneWord.clear(); OBJi++; break;}
    }

    while(true)
    {
        if(oneLine[OBJi] != '/' && oneLine[OBJi] != ' ') {oneWord += oneLine[OBJi]; OBJi++;}
        else {indexUVs = std::stoi(oneWord); oneWord.clear(); OBJi++; break;}
    }

    while(true)
    {
        if(oneLine[OBJi] != ' ' && oneLine[OBJi] != ' ') {oneWord += oneLine[OBJi]; OBJi++;}
        else {indexNorms = std::stoi(oneWord); oneWord.clear(); OBJi++; break;}
    }

    int posIndex = indexPos -1;
    int normalsIndex = indexNorms -1;
    int texcoordsIndex = indexUVs -1;

    mVertices.push_back
            (
                Vertex
                (
                    importedPos[posIndex].GetX(),
                    importedPos[posIndex].GetY(),
                    importedPos[posIndex].GetZ(),
                    importedNorms[normalsIndex].GetX(),
                    importedNorms[normalsIndex].GetY(),
                    importedNorms[normalsIndex].GetZ(),
                    importedUVs[texcoordsIndex].x(),
                    importedUVs[texcoordsIndex].y()
                )
            );

    mIndices.push_back(indices++);
}

void objMesh::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, (GLfloat*)mTransformationMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    updateBoundingBox();
}

void objMesh::moveAlongCurve(unsigned int increment, std::vector<Vector3D> points, VisualObject* ground)
{
    mTransformationMatrix.setToIdentity();

    if(increment < points.size())
    {
        mTransformation.translation.setX(points.at(increment).x);
        mTransformation.translation.setY(points.at(increment).y);
        mTransformation.translation.setZ(points.at(increment).z);
    }
    barycentricMove(mTransformation.translation.x(),
                    mTransformation.translation.y(),
                    mTransformation.translation.z(), ground);
    mTransformationMatrix.scale(10,10,10);
}

void objMesh::makeFrustum(Vector3D cornerNear, Vector3D cornerFar)
{
        mVertices.insert( mVertices.end(),
          {//Vertex data for front points       color                       uv
           Vertex{-cornerNear.x, -cornerNear.y, cornerNear.z,       1, 0.301, 0.933,          0.f, 0.f},     // 0
           Vertex{cornerNear.x,  -cornerNear.y, cornerNear.z,       1, 0.301, 0.933,          0.f, 0.f},
           Vertex{cornerNear.x,  cornerNear.y,  cornerNear.z,       1, 0.301, 0.933,          0.f, 0.f},
           Vertex{-cornerNear.x, cornerNear.y,  cornerNear.z,       1, 0.301, 0.933,          0.f, 0.f},
           //Vertex data for back
           Vertex{-cornerFar.x, -cornerFar.y, cornerFar.z,      1, 0.301, 0.933,          0.f, 0.f},    // 4
           Vertex{cornerFar.x,  -cornerFar.y, cornerFar.z,      1, 0.301, 0.933,          0.f, 0.f},
           Vertex{cornerFar.x,  cornerFar.y,  cornerFar.z,      1, 0.301, 0.933,          0.f, 0.f},
           Vertex{-cornerFar.x, cornerFar.y,  cornerFar.z,      1, 0.301, 0.933,          0.f, 0.f},
                          });

        //One line at a time
        mIndices.insert( mIndices.end(),
        { 0, 1, 1, 2, 2, 3, 3, 0,       //front rectangle
          4, 5, 5, 6, 6, 7, 7, 4,       //back rectangle
          0, 4, 3, 7,                   //leftside lines
          1, 5, 2, 6                    //rightside lines
                         });

}
