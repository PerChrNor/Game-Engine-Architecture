#ifndef CAMERA_H
#define CAMERA_H

#include "Math/matrix4x4.h"
#include "QMatrix4x4"
#include "Math/vector3d.h"
#include "Math/vector4d.h"
#include "visualobject.h"

class Camera: public QMatrix4x4, public VisualObject
{
private:
    Vector3D m_Forward{0.f, 0.f, -1.f};
    Vector3D m_Right{1.f, 0.f, 0.f};
    Vector3D m_Up{0.f, 1.f, 0.f};
    Vector3D m_Position{0.f, 0.f, 0.f};

    float m_yaw = 0;
    float m_pitch = 0;

    Matrix4x4 m_YawMatrix;
    Matrix4x4 m_PitchMatrix;
    float m_moveSpeed{0.f};



public:
    Camera();

    void pitch(float degrees);
    void yaw(float degrees);
    void updateRVector();
    void updateFVector(float yawDegree,float pitchDegree);
    void updateUpVector();
    void update();
    void setPosition(const Vector3D &position);
    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);
    virtual void init() override;
    virtual void draw() override;

    Matrix4x4 m_ViewMatrix;
    Matrix4x4 m_ProjectionMatrix;

    Vector3D position() const;
    Vector3D up() const;
    Vector3D FVec() const;

    float horizontalDistFromPlayer = 6;

    /// \brief mouseUpdate sets the rotation of the camera based on the mouse movement.
    /// It saves the mouse position on in the window, then rotates the camera by how many pixels the camera has moved since the last frame.
    /// The pixels is multiplied by a mouse speed to make the rotation feel more manageable.
    /// The forward vector, the right vector and the up vector is then updated.
    /// \param newMousePosition is how far the mouse have traveled on the screen from the last frame.
    void mouseUpdate(const QVector2D& newMousePosition);

    /// \brief oldMousePosition is the mouse position in the previous frame.
    QVector2D oldMousePosition;

    /// \brief cameraRotationSpeed is the value the mouseUpdate() function uses to change speed.
    float cameraRotationSpeed = 0.1f;
    Vector3D at;
    bool isEditorCamera = true;

private:
    QMatrix4x4 QtempYawMatrix;
    QMatrix4x4 QtempPitchMatrix;


};

#endif // CAMERA_H
