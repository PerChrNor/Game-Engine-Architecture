#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>

#include "vertex.h"
#include "visualobject.h"
#include "interactiveobject.h"

#include "Math/matrix4x4.h"
#include "Shaders/mousepickershader.h"
#include "Shaders/skyboxshader.h"
#include "skybox.h"

#include "Objects/xyz.h"
#include "Objects/triangle.h"

#include "camera.h"
#include "light.h"
#include "texture.h"
#include "objmesh.h"

#include "coreengine.h"
#include "resourcemanager.h"
#include "rendersystem.h"
#include "particlesystem.h"
#include "collisions.h"
#include "scriptingsystem.h"
#include "movementsystem.h"
#include "frustumclass.h"


class QOpenGLContext;
class Shader;
class MainWindow;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials

/// The core engine of the project.
///
/// In RenderWindow all the functionality from all the other classes is pulled together to create and run the scene.
/// It creates, calls for initalization and calls for drawing of the objects needed.
/// It also calls many of the function that handle the program logic in addition to dealing with some of the logic itself.
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);

    QWidget *mRenderWindowContainer{nullptr};
    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;
    void checkForGLerrors();

    void switchScene();
    void changeRender();

    void renderEditorObjects();
    void mousePicking();
    void setPickedObject(int pickedID);
    void cancelPickedObject();
    void setFocus();

    bool wireFrame = false;
    bool editor = true;
    bool drawXYZ = true;
    bool drawTriangle = false;
    void SpawnNewObject();
    int NewTriangleNumber = 0;
    bool soundOn = false;

    VisualObject * mEditorObjects[EditorObjectNumber]{nullptr};
    class Scene *mScene1{nullptr};
    class CoreEngine *mEngine{nullptr};
    ResourceManager *mManager;
    renderSystem *mRenderer;

    Collisions* mCollisionManager{nullptr};
    // Camera objects
    Camera *mCamera;
    Camera *mGameCamera;
    SkyBox *mSkyBox;

    FrustumClass *mFrustum;

    movementSystem *mMovement{nullptr};
    float mCarSpeed{0.2f};

private slots:
    void render();

private:
    void init();
    std::vector<Vertex>mVertices;

    QOpenGLContext *mContext;
    bool mInitialized;

    Texture *mTexture[4]{nullptr};
    Shader *mShaderProgram[4]{nullptr};
    qreal mRetinaScale{1};  //used on "double pixle/retina" displays
    ParticleSystem *mParticleSystem;
    float mTime;

    void shaderSetup();

    QTimer *mRenderTimer;           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow;        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void calculateFramerate();
    void startOpenGLDebugger();
    void editorMode();
    void playMode();
    void setCameraSpeed(float value);
    void monkeyMovement();


    // Objects in programming folder
    XYZ xyz;
    light* mLight{nullptr};


    Triangle mTriangle;
    Triangle *newTriangle[20];



    // Beziercurve objects

    float screenWidth;
    float screenHeight;
    float mAspectratio{1.f};

    bool mouseInput = false;

    float mCameraSpeed{0.5f};

    // Collision
    void checkCollision();
    void restartGame();

    int trophiesCollected = 0;

    bool mousepick = false;

    float cameraXPOS{0};
    float cameraZPOS{0};


    bool cameraTurnable = false;

    void setupTrack();
    void thirdPersonCamMovement();

    void runFrustum();

protected:
    //The QWindow that we inherit from has these functions to capture
    //mouse and keyboard. Uncomment to use (you also have to make the definitions of these functions
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}

    void mouseMoveEvent(QMouseEvent*);

};

#endif // RENDERWINDOW_H
