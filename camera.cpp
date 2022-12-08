#include "camera.h"

Camera::Camera()
{
    m_ViewMatrix.setToIdentity();
    m_ProjectionMatrix.setToIdentity();

    m_YawMatrix.setToIdentity();
    m_PitchMatrix.setToIdentity();

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 0, 0, 1));
    mVertices.push_back(Vertex(0.5,-0.5,-0.5    , 0, 0, 1));
    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 0, 1));

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 0, 1, 0));
    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 1, 0));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 0, 1, 0));

    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 1, 0, 0));
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 1, 0, 0));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 1, 0, 0));

    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 0, 0, 1));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 0, 0, 1));
    mVertices.push_back(Vertex(-0.5,0.5,-1.5   , 0, 0, 1));

    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 0, 1, 0));
    mVertices.push_back(Vertex(-0.5,0.5,-1.5   , 0, 1, 0));
    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 0, 1, 0));

    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 1, 0, 0));
    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 1, 0, 0));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 1, 0, 0));

    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 0, 1));
    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 0, 0, 1));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 0, 0, 1));

    mVertices.push_back(Vertex(0.5,-0.5,-0.5    , 0, 1, 0));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 0, 1, 0));
    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 0, 1, 0));

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 1, 0, 0));
    mVertices.push_back(Vertex(-0.5,-0.5,-1.5  , 1, 0, 0));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 1, 0, 0));

    mVertices.push_back(Vertex(0.5,-0.5,-0.5    , 0, 0, 1));
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5   , 0, 0, 1));
    mVertices.push_back(Vertex(0.5,-0.5,-1.5   , 0, 0, 1));

    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 0, 1, 0));
    mVertices.push_back(Vertex(-0.5,0.5,-1.5   , 0, 1, 0));
    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 0, 1, 0));

    mVertices.push_back(Vertex(0.5,0.5,-0.5     , 1, 0, 0));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5    , 1, 0, 0));
    mVertices.push_back(Vertex(0.5,0.5,-1.5    , 1, 0, 0));


}

void Camera::pitch(float degrees)
{
    m_pitch -= degrees;
    //updateFVector();

}

void Camera::yaw(float degrees)
{
    m_yaw -= degrees;
    //updateFVector();

}

void Camera::updateRVector()
{
    QVector3D tempRVec;
    QMatrix4x4 tempMatrix;
    tempMatrix.setToIdentity();

    tempRVec.setX(m_Right.x); tempRVec.setY(m_Right.y); tempRVec.setZ(m_Right.z);

    tempMatrix.rotate(m_yaw, QVector3D(0,1,0));

    tempRVec = tempMatrix * tempRVec;
    tempRVec.normalize();

    m_Right.x = tempRVec.x(); m_Right.y = tempRVec.y(); m_Right.z = tempRVec.z();

    m_Right.normalize();

    updateUpVector();
}

void Camera::updateFVector(float yawDegree,float pitchDegree)
{

//    QVector3D tempFVec;
//    QMatrix4x4 tempFMatrix;


//    tempFMatrix.setToIdentity();
//   tempFVec.setX(m_Forward.x); tempFVec.setY(m_Forward.y); tempFVec.setZ(m_Forward.z);
//    tempFVecPitchRot = m_Forward;

//    tempFMatrix.rotate(degree,QVector3D(0,1,0));
//    //tempMatrix.rotate(m_pitch, QVector3D(1,0,0));
//    tempFVec = tempFMatrix * tempFVec;

//    tempFVec.normalize();


   QVector3D tempFVec;
   QVector3D tempUpVec;
   QVector3D tempRVec;

   tempFVec.setX(m_Forward.x);  tempFVec.setY(m_Forward.y);  tempFVec.setZ(m_Forward.z);
   tempUpVec.setX(m_Up.x);      tempUpVec.setY(m_Up.y);      tempUpVec.setZ(m_Up.z);
   tempRVec.setX(m_Right.x);    tempRVec.setY(m_Right.y);     tempRVec.setZ(m_Right.z);


    QtempYawMatrix.setToIdentity();
    QtempPitchMatrix.setToIdentity();


    QtempYawMatrix.column(3) = tempFVec;
    QtempYawMatrix.rotate(yawDegree, QVector3D(0,1,0));
    tempFVec = tempFVec * QtempYawMatrix;

    tempUpVec = QVector3D::crossProduct(tempFVec, tempUpVec);
    QtempPitchMatrix.column(3) = tempFVec;
    QtempPitchMatrix.rotate(pitchDegree, tempUpVec);


    QMatrix4x4 temp4x4;
    temp4x4.setToIdentity();
    temp4x4 = QtempYawMatrix * QtempPitchMatrix;
    tempFVec = tempFVec * temp4x4;
    tempFVec.normalize();

    m_Forward.x = tempFVec.x(); m_Forward.y = tempFVec.y(); m_Forward.z = tempFVec.z();

    //qDebug() << m_Forward.x << m_Forward.y << m_Forward.z;

    updateRVector();
}

void Camera::updateUpVector(){

    m_Up = m_Forward^m_Right;
}


void Camera::update()
{
    m_YawMatrix.setToIdentity();
    m_PitchMatrix.setToIdentity();

//    m_PitchMatrix.rotateX(m_pitch);
//    m_YawMatrix.rotateY(m_yaw);


    m_Position -= m_Forward * m_moveSpeed;

    //m_ViewMatrix = m_PitchMatrix * m_YawMatrix;
    m_ViewMatrix.translate(-m_Position);
    if (isEditorCamera == true)
    {
        at = m_Position + m_Forward;
    }
    m_ViewMatrix.lookAt(m_Position, at, m_Up);
    //mTransformationMatrix.translate(m_Position.GetX() - mTransformationMatrix.column(3).x(), m_Position.GetY() - mTransformationMatrix.column(3).y(), m_Position.GetZ() - mTransformationMatrix.column(3).z());
    m_moveSpeed = 0;
}

void Camera::setPosition(const Vector3D &position)
{
    m_Position = position;
}

void Camera::setSpeed(float speed)
{
    m_moveSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
    m_Position.y += deltaHeigth;
}

void Camera::moveRight(float delta)
{

    Vector3D StrafeDirection = m_Forward^m_Up;
    m_Position += StrafeDirection * delta;

}

void Camera::init()
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

    mTransformationMatrix.setToIdentity();
}

void Camera::draw()
{
    glBindVertexArray( mVAO );

    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, (GLfloat*)mTransformationMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

Vector3D Camera::position() const
{
    return m_Position;
}

Vector3D Camera::up() const
{
    return m_Up;
}

Vector3D Camera::FVec() const{
    return m_Forward;
}



void Camera::mouseUpdate(const QVector2D &newMousePosition){



    QVector2D mouseDelta = newMousePosition - oldMousePosition;
    if(mouseDelta.length() > 30.0f){
        oldMousePosition = newMousePosition;
        return;

    }

    QVector3D tempFVec;
    QVector3D tempUpVec;
    QVector3D tempRVec;

    tempFVec.setX(m_Forward.x);  tempFVec.setY(m_Forward.y);  tempFVec.setZ(m_Forward.z);
    tempUpVec.setX(m_Up.x);      tempUpVec.setY(m_Up.y);      tempUpVec.setZ(m_Up.z);
    tempRVec.setX(m_Right.x);    tempRVec.setY(m_Right.y);     tempRVec.setZ(m_Right.z);

    QVector3D ToRotateAround = QVector3D::crossProduct(tempFVec,tempUpVec);

    QMatrix4x4 rotatorA;
    rotatorA.setToIdentity();
    rotatorA.rotate(mouseDelta.x() * cameraRotationSpeed, tempUpVec);
    QMatrix4x4 rotatorB;
    rotatorB.setToIdentity();
    rotatorB.rotate(mouseDelta.y() * cameraRotationSpeed, ToRotateAround);
    QMatrix4x4 rotatorC = rotatorA * rotatorB;

    tempFVec = rotatorC * tempFVec;

    m_Forward.x = tempFVec.x(); m_Forward.y = tempFVec.y(); m_Forward.z = tempFVec.z();

    oldMousePosition = newMousePosition;
}
