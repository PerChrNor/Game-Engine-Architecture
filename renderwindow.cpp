#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <iostream>

#include "Shaders/shader.h"
#include "Shaders/plainshader.h"
#include "Shaders/phongshader.h"
#include "Shaders/textureshader.h"
#include "Shaders/particleshader.h"
#include "mainwindow.h"
#include "Math/vector2d.h"
#include "Math/vector3d.h"
#include "Math/math_constants.h"
#include "ui_mainwindow.h"

#include "resourcemanager.h"
#include "Objects/grid_area.h"
#include "scene.h"
#include "rendersystem.h"
#include "coreengine.h"
#include "constants.h"
#include "frustumclass.h"


RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

    mManager = &ResourceManager::getInstance();

    ResourceManager::getInstance().soundInit();
    mRenderer = new renderSystem();

    mMovement = new movementSystem();
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{

    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }
    if (!mInitialized)
        mInitialized = true;

    initializeOpenGLFunctions();
    startOpenGLDebugger();

    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    shaderSetup();

    VisualObject *temp;
    temp = new Grid_Area(20);
    temp->init();
    mEditorObjects[GRID] = temp;

    temp = new XYZ();
    temp->init();
    mEditorObjects[AXIS] = temp;

    mLight = new light();
    mLight->init();
    mLight->mTransformationMatrix.translate(1, 1, 0);
    qDebug() << "light pos" << mLight->mTransformationMatrix;

    mScene1 = new Scene(this);
    mScene1->mRenderSystem = mRenderer;
    mScene1->mName = "Scene 1";

    mCamera = new Camera();
    mCamera->setPosition(Vector3D(0.f, 2.f, 10.f));
    mCamera->isEditorCamera = true;

    mSkyBox = new SkyBox();
    mSkyBox->init();
    mSkyBox->mMaterial = ResourceManager::getInstance().mMaterial[6];

    mParticleSystem = new ParticleSystem();
    mParticleSystem->init();
    mParticleSystem->mMaterial = ResourceManager::getInstance().mMaterial[5];
    //sets up particle system variables
    mParticleSystem->mEmitter.mSizeBegin = 0.5f,
            mParticleSystem->mEmitter.mSizeAlt = 0.3f,
            mParticleSystem->mEmitter.mSizeEnd = 0.0f;
    mParticleSystem->mEmitter.mLifeSpan = 1.0f;
    mParticleSystem->mEmitter.mVelocity = {500, 500, 0};
    mParticleSystem->mEmitter.mVelocityAlt = {3, 3, 0};
    mParticleSystem->mEmitter.mPos = {4, 3, 0};

    mGameCamera = new Camera();
    mGameCamera->setPosition(Vector3D(0.f, 2.f, 3.f));
    mGameCamera->isEditorCamera = false;

    mFrustum = new FrustumClass();
    mFrustum->saveCamera(mGameCamera);

    setupTrack();

    // Updating the outliner
    mMainWindow->updateUI(mManager->mEntities);


    static_cast<PlainShader*>(ResourceManager::getInstance().mShaderProgram[PLAINSHADER])->CurrentCamera = mCamera;
    static_cast<TextureShader*>(ResourceManager::getInstance().mShaderProgram[TEXTURESHADER])->CurrentCamera = mCamera;
    static_cast<PhongShader*>(ResourceManager::getInstance().mShaderProgram[PHONGSHADER])->mCurrentLight = mLight;
    static_cast<PhongShader*>(ResourceManager::getInstance().mShaderProgram[PHONGSHADER])->CurrentCamera = mCamera;
    static_cast<ParticleShader*>(ResourceManager::getInstance().mShaderProgram[PARTICLESHADER])->CurrentCamera = mCamera;
    static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[MOUSEPICKERSHADER])->CurrentCamera = mCamera;
    static_cast<SkyboxShader*>(ResourceManager::getInstance().mShaderProgram[SKYBOXSHADER])->CurrentCamera = mCamera;

    mScene1->mRenderSystem->init();

    mManager->stopAllSound();

    mManager->CheckEntitiesWithCollider();
    mManager->CheckEntitiesWithStaticDynamicCollider();

    //CollisionManager
    mCollisionManager = new Collisions;
    mCollisionManager->mCurrentManager = mManager;
    mCollisionManager->initCollisions();

    cameraXPOS = mManager->mEntities[0].mTransform.position.x();
    cameraZPOS = mManager->mEntities[0].mTransform.position.z() + mGameCamera->horizontalDistFromPlayer;

    mMovement->mCurrentManager = mManager;
    mMovement->mCarspeed = mCarSpeed;
}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    if (editor == true)
    {
        mCamera->update();

        renderEditorObjects();
    }
    else
    {
        thirdPersonCamMovement();
        mGameCamera->setPosition(Vector3D(cameraXPOS, mManager->mEntities[0].mTransform.position.y()+2, cameraZPOS));
        mGameCamera->at.x = mManager->mEntities[0].mTransform.position.x();
        mGameCamera->at.y = mManager->mEntities[0].mTransform.position.y() + 1;
        mGameCamera->at.z = mManager->mEntities[0].mTransform.position.z();
        mGameCamera->update();
    }


    // Runs the extraction of the frustum, and then culls
    //runFrustum();

    mManager->updateListener(mCamera->position(),Vector3D(0,0,0),mCamera->FVec(),mCamera->up());

    mParticleSystem->Emitter();
    mParticleSystem->ParticleUpdate(mTime);
    mParticleSystem->draw();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mMainWindow->renderMousePick == false)
    {    //Render Editor assets:
        if (editor == true)
        renderEditorObjects();
        mScene1->render(mManager);
        //mSkyBox->draw();
        mContext->swapBuffers(this);
        mLight->draw();

    }
    else if(mMainWindow->renderMousePick == true)
    {
        mousePicking();
        //mScene1->render(mManager);
        mContext->swapBuffers(this);
    }

    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    //mContext->swapBuffers(this);

    mManager->checkCollisions();
    mCollisionManager->checkAllCollisions();

}

void RenderWindow::shaderSetup()
{
    //ResourceManager &tempRes = ResourceManager::getInstance();

    mManager->readShaders(this);
    mManager->readTextures();
    //tempRes.readObjs();
    mManager->makeMaterials();
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }

    mAspectratio = static_cast<float>(width()) / height();
    mCamera->m_ProjectionMatrix.perspective(60.f, mAspectratio, 0.1, 100.f);
    mGameCamera->m_ProjectionMatrix.perspective(60.f, mAspectratio, 0.1, 100.f);
}

void RenderWindow::changeRender()
{
    if (wireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        qDebug() << "Wireframe mode on" << '\n';
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        qDebug() << "Wireframe mode off" << '\n';
    }
}

void RenderWindow::renderEditorObjects()
{
    glUseProgram(ResourceManager::getInstance().mShaderProgram[PLAINSHADER]->mProgram);

    glUniformMatrix4fv(ResourceManager::getInstance().mShaderProgram[PLAINSHADER]->vMatrixUniform, 1, GL_TRUE, mCamera->m_ViewMatrix.constData());
    glUniformMatrix4fv(ResourceManager::getInstance().mShaderProgram[PLAINSHADER]->pMatrixUniform, 1, GL_TRUE, mCamera->m_ProjectionMatrix.constData());

    if (drawXYZ)
    {
        glUniformMatrix4fv(ResourceManager::getInstance().mShaderProgram[PLAINSHADER]->mMatrixUniform, 1, GL_TRUE, mEditorObjects[1]->mMatrix.constData());
        mEditorObjects[GRID]->draw();
        glUniformMatrix4fv(ResourceManager::getInstance().mShaderProgram[PLAINSHADER]->mMatrixUniform, 1, GL_TRUE, mEditorObjects[2]->mMatrix.constData());
        mEditorObjects[AXIS]->draw();
    }

}
//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if(mCameraSpeed > 0.3f)
        mCameraSpeed = 0.3f;
}

void RenderWindow::restartGame()
{

}
///
/// \brief setFocus, set the focus for mousepicker.
/// by candidate 8003.
///
void RenderWindow::setFocus()
{
    mMainWindow->mRenderWindowContainer->setFocus();
}

/// Creates the mousepicking colors and reads the color under the cursor to check which entity is picked.
///
/// Function starts by calling function to render the scene in shades of red. After this is done the function reads the color values under the cursor and restore these to a ID value which the
/// entity was given during rendering. This ID is then compared to the ID's of all entities and the corresponding entity is picked in the outliner.
/// This function is activated by clicking on an entity.
/// Written by candidate 8008 and 8003.
/// @see renderSystem::renderMousePicker().
void RenderWindow::mousePicking()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();    //must call this every frame it seems...
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    QMatrix4x4 temp;
    static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[MOUSEPICKERSHADER])->use(temp, nullptr);
    checkForGLerrors();

    mScene1->renderMousePicker(static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[MOUSEPICKERSHADER])->mPickerColor,
                               mManager);
//    checkForGLerrors();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //configure how glReadPixels will behave with respect to memory alignment
//    checkForGLerrors();

    glFlush();  //Force all GL-commands to be sent
    glFinish(); //Force GL to finish rendering

//    checkForGLerrors();

    // Read the color of the pixel at the mouse cursor.
    // Ultra-mega-over slow, even for 1 pixel,
    // because the framebuffer is on the GPU.
    unsigned char data[4];

    QPoint cursorPosition   = this->mapFromGlobal(this->cursor().pos());    //global mouse position converted to local
//    qDebug() << "pos" << cursorPosition;

    glReadPixels(cursorPosition.x()*mRetinaScale, (height()- cursorPosition.y())*mRetinaScale , 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    qDebug() << "Height" << height() * mRetinaScale << "Height - y" << (height()- cursorPosition.y())*mRetinaScale;
    int pickedID =
        data[0] +
        data[1] * 256 +
        data[2] * 256*256;

    if (pickedID < 100000)
    {
        for (unsigned int i = 0; i < mRenderer->mNumberOfMeshes; i++)
        {   
            if (pickedID == mManager->mEntities[i].mMaterialID.mousePickerID)
            {
                mMainWindow->selectObjectInOutliner(mManager->mEntities[i].mName.c_str());

            }
        }
    }
    setFocus();
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        mouseInput = true;
        cameraTurnable = true;
    }
    if (event->button() == Qt::LeftButton)
    {
        mousePicking();
    }
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        mouseInput = false;
        cameraTurnable = false;
    }
    if (event->button() == Qt::LeftButton)
    {

    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    //Shuts down whole program
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();
    }

    mCamera->setSpeed(0.f);
    if (mouseInput == true)
    {
        if (event->key() == Qt::Key_W)
        {
            mCamera->setSpeed(-mCameraSpeed);
        }
        if (event->key() == Qt::Key_S)
        {
            mCamera->setSpeed(mCameraSpeed);
        }
        if (event->key() == Qt::Key_D)
        {
            mCamera->moveRight(mCameraSpeed);
        }
        if (event->key() == Qt::Key_A)
        {
            mCamera->moveRight(-mCameraSpeed);
        }
        if (event->key() == Qt::Key_F)
        {
            mCamera->updateHeigth(-mCameraSpeed);
        }
        if (event->key() == Qt::Key_R)
        {
            mCamera->updateHeigth(mCameraSpeed);
        }
    }
    else if (mouseInput == false && editor == false)
    {
        if (event->key() == Qt::Key_W)
        {
            mMovement->moveForward(true);
            mCollisionManager->setPlayerheight(mCollisionManager->playerHeight);
            cameraZPOS -= 0.2;
            mManager->updateColliderPos(0);
        }
        if (event->key() == Qt::Key_S)
        {
            mMovement->moveForward(false);
            mCollisionManager->setPlayerheight(mCollisionManager->playerHeight);
            cameraZPOS += 0.2;
            mManager->updateColliderPos(0);
        }
        if (event->key() == Qt::Key_D)
        {
            mManager->mEntities[0].mTransform.rotation.setY(mManager->mEntities[0].mTransform.rotation.y() - 1);
            mMovement->updateFVector(1);
        }
        if (event->key() == Qt::Key_A)
        {
          mManager->mEntities[0].mTransform.rotation.setY(mManager->mEntities[0].mTransform.rotation.y() + 1);
            mMovement->updateFVector(-1);
        }
    }
}

void RenderWindow::SpawnNewObject()           // does not currently work
{
    unsigned short object = mManager->makeEntity("monkey2");
    mManager->attachMesh("../GEA2020/Assets/monkey.obj", object, mRenderer);
    mManager->attachTransform(transformComponent(), object);
    mMainWindow->updateUI(mManager->mEntities);
}

void RenderWindow::mouseMoveEvent(QMouseEvent * e){

    QVector2D in;
    in.setX(e->x());
    in.setY(e->y());
    if(cameraTurnable){
     mCamera->mouseUpdate(in);
    }


    //qDebug() << in.x() << " " << in.y() <<"/n";
}

/// This function creates the entities used in the scene.
///
/// The setupTrack function creates all the entities needed to render the scene. It also attaches the needed components and places them at their correct starting position.
/// Written by candidate 8008 and 8002.
/// @see ResourceManager
void RenderWindow::setupTrack()
{
    // Player
    unsigned short object = mManager->makeEntity("Car1", "Player");
    mManager->attachMesh("../GEA2020/Assets/vroomvroom.obj", object, mRenderer);
    mManager->attachTransform(transformComponent(), object);
    mManager->attachSound("Car Noice",object,"../GEA2020/Assets/engine-loop2.wav",true,1.0f);
    mManager->attachSphereCollider(object,"Dynamic",5.0f);
    mManager->attachMaterial(2, object);
    mManager->mEntities[object].mTransform.position = QVector3D(0.f, 1.0f, 0.f);
    mManager->mEntities[object].mTransform.rotation = QVector3D(0.f, 180.f, 0.f);

    // Track pieces
    unsigned short roadStraight = mManager->makeEntity("Straight1", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSRoad.obj", roadStraight, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight);
    mManager->attachMaterial(0, roadStraight);
    mManager->mEntities[roadStraight].mTransform.position = QVector3D(0.0f, 0.0f, 0.0f);
    mManager->attachBoxCollider(roadStraight,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadTurn = mManager->makeEntity("Turn1", "Track");
    mManager->attachMesh("../GEA2020/Assets/NWTurn.obj", roadTurn, mRenderer);
    mManager->attachTransform(transformComponent(), roadTurn);
    mManager->attachMaterial(0, roadTurn);
    mManager->mEntities[roadTurn].mTransform.position = QVector3D(0.0f, 8.0f, -24.0f);
    mManager->attachBoxCollider(roadTurn,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadSlope = mManager->makeEntity("Slope1", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSUPS.obj", roadSlope, mRenderer);
    mManager->attachTransform(transformComponent(), roadSlope);
    mManager->attachMaterial(0, roadSlope);
    mManager->mEntities[roadSlope].mTransform.position = QVector3D(0.0f, 0.0f, -17.0f);
    mManager->attachBoxCollider(roadSlope,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadStraight2 = mManager->makeEntity("Straight2", "Track");
    mManager->attachMesh("../GEA2020/Assets/EWRoad.obj", roadStraight2, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight2);
    mManager->attachMaterial(0, roadStraight2);
    mManager->mEntities[roadStraight2].mTransform.position = QVector3D(18.0f, 8.0f, -37.4f);
    mManager->attachBoxCollider(roadStraight2,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadTurn2 = mManager->makeEntity("Turn2", "Track");
    mManager->attachMesh("../GEA2020/Assets/NETurn.obj", roadTurn2, mRenderer);
    mManager->attachTransform(transformComponent(), roadTurn2);
    mManager->attachMaterial(0, roadTurn2);
    mManager->mEntities[roadTurn2].mTransform.position = QVector3D(23.0f, 8.0f, -37.4f);
    mManager->attachBoxCollider(roadTurn2,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadStraight3 = mManager->makeEntity("Straight3", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSRoad.obj", roadStraight3, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight3);
    mManager->attachMaterial(0, roadStraight3);
    mManager->mEntities[roadStraight3].mTransform.position = QVector3D(36.6f, 7.9f, -19.0f);
    mManager->attachBoxCollider(roadStraight3,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadSlope2 = mManager->makeEntity("Slope2", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSUPS.obj", roadSlope2, mRenderer);
    mManager->attachTransform(transformComponent(), roadSlope2);
    mManager->attachMaterial(0, roadSlope2);
    mManager->mEntities[roadSlope2].mTransform.position = QVector3D(36.6f, 0.0f, -7.0f);
    mManager->attachBoxCollider(roadSlope2,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadTurn3 = mManager->makeEntity("Turn3", "Track");
    mManager->attachMesh("../GEA2020/Assets/SWTurn.obj", roadTurn3, mRenderer);
    mManager->attachTransform(transformComponent(), roadTurn3);
    mManager->attachMaterial(0, roadTurn3);
    mManager->mEntities[roadTurn3].mTransform.position = QVector3D(50.0f, 0.0f, 17.0f);
    mManager->attachBoxCollider(roadTurn3,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadTurn4 = mManager->makeEntity("Turn4", "Track");
    mManager->attachMesh("../GEA2020/Assets/NETurn.obj", roadTurn4, mRenderer);
    mManager->attachTransform(transformComponent(), roadTurn4);
    mManager->attachMaterial(0, roadTurn4);
    mManager->mEntities[roadTurn4].mTransform.position = QVector3D(49.0f, 0.0f, 17.0f);
    mManager->attachBoxCollider(roadTurn4,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadTurn5 = mManager->makeEntity("Turn5", "Track");
    mManager->attachMesh("../GEA2020/Assets/SETurn.obj", roadTurn5, mRenderer);
    mManager->attachTransform(transformComponent(), roadTurn5);
    mManager->attachMaterial(0, roadTurn5);
    mManager->mEntities[roadTurn5].mTransform.position = QVector3D(62.4f, 0.0f, 30.0f);
    mManager->attachBoxCollider(roadTurn5,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadStraight4 = mManager->makeEntity("Straight4", "Track");
    mManager->attachMesh("../GEA2020/Assets/EWRoad.obj", roadStraight4, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight4);
    mManager->attachMaterial(0, roadStraight4);
    mManager->mEntities[roadStraight4].mTransform.position = QVector3D(44.f, 0.0f, 43.4f);
    mManager->attachBoxCollider(roadStraight4,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadSlope3 = mManager->makeEntity("Slope3", "Track");
    mManager->attachMesh("../GEA2020/Assets/EWUPW.obj", roadSlope3, mRenderer);
    mManager->attachTransform(transformComponent(), roadSlope3);
    mManager->attachMaterial(0, roadSlope3);
    mManager->mEntities[roadSlope3].mTransform.position = QVector3D(30.6f, 0.0f, 43.4f);
    mManager->attachBoxCollider(roadSlope3,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadSlope4 = mManager->makeEntity("Slope4", "Track");
    mManager->attachMesh("../GEA2020/Assets/EWUPE.obj", roadSlope4, mRenderer);
    mManager->attachTransform(transformComponent(), roadSlope4);
    mManager->attachMaterial(0, roadSlope4);
    mManager->mEntities[roadSlope4].mTransform.position = QVector3D(18.0f, 0.0f, 43.4f);
    mManager->attachBoxCollider(roadSlope4,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadTurn6 = mManager->makeEntity("Turn6", "Track");
    mManager->attachMesh("../GEA2020/Assets/SWTurn.obj", roadTurn6, mRenderer);
    mManager->attachTransform(transformComponent(), roadTurn6);
    mManager->attachMaterial(0, roadTurn6);
    mManager->mEntities[roadTurn6].mTransform.position = QVector3D(13.4f, 0.0f, 46.0f);
    mManager->attachBoxCollider(roadTurn6,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadStraight5 = mManager->makeEntity("Straight5", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSRoad.obj", roadStraight5, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight5);
    mManager->attachMaterial(0, roadStraight5);
    mManager->mEntities[roadStraight5].mTransform.position = QVector3D(0.0f, 0.0f, 28.0f);
    mManager->attachBoxCollider(roadStraight5,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadStraight6 = mManager->makeEntity("Straight6", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSRoad.obj", roadStraight6, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight6);
    mManager->attachMaterial(0, roadStraight6);
    mManager->mEntities[roadStraight6].mTransform.position = QVector3D(0.0f, 0.0f, 18.0f);
    mManager->attachBoxCollider(roadStraight6,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));

    unsigned short roadStraight7 = mManager->makeEntity("Straight7", "Track");
    mManager->attachMesh("../GEA2020/Assets/NSRoad.obj", roadStraight7, mRenderer);
    mManager->attachTransform(transformComponent(), roadStraight7);
    mManager->attachMaterial(0, roadStraight7);
    mManager->mEntities[roadStraight7].mTransform.position = QVector3D(0.0f, 0.0f, 10.0f);
    mManager->attachBoxCollider(roadStraight7,"Static",Vector3D(20,20,20), Vector3D(-20,-20,-20));
}

/// Makes the mGameCamera work as a third person camera on the player character.
///
/// This function calculates and sets the position of the mGameCamera as a traditional third person camera for the player character.
/// This includes following the rotation of the player. This is done by calculating the offset for the x and z axis and subtracting these values from the position of the player.
/// Written by candidate 8008
void RenderWindow::thirdPersonCamMovement()
{
    float theta = mManager->mEntities[0].mTransform.rotation.y();
    float offsetX = mGameCamera->horizontalDistFromPlayer * sinf(deg2radf(theta));
    float offsetZ = mGameCamera->horizontalDistFromPlayer * cosf(deg2radf(theta));
    cameraXPOS = mManager->mEntities[0].mTransform.position.x() - offsetX;
    cameraZPOS = mManager->mEntities[0].mTransform.position.z() - offsetZ;
}

/// Runs the extractFrustum() function, then checks for collisions to cull.
///
/// The function will first run extractFrustum() on mGameCamera specifically.
/// Then it will go through a for-loop to check if there are any collisions inside to cull.
void RenderWindow::runFrustum()
{
    mFrustum->extractFrustum();

    for ( unsigned int i=0; i < mRenderer->mNumberOfMeshes; i++ )
    {
        // Checks if the bounding collider is a sphere.
        /*if (mManager->mEntities[i].mCollider.mShape == "Sphere")
        {
        mManager->mEntities[i].mMesh.isInFrustum = mFrustum->sphereInFrustum(mManager->mEntities[i].mTransform.position.x(),
                                                                             mManager->mEntities[i].mTransform.position.y(),
                                                                             mManager->mEntities[i].mTransform.position.z(),
                                                                             mManager->mEntities[i].mCollider.mRadius);
        }*/

        // Checks if the bounding collider is a box.
        if (mManager->mEntities[i].mCollider.mShape == "Box")
        {
        mManager->mEntities[i].mMesh.isInFrustum = mFrustum->cubeInFrustum(mManager->mEntities[i].mTransform.position.x(),
                                                                           mManager->mEntities[i].mTransform.position.y(),
                                                                           mManager->mEntities[i].mTransform.position.z(),
                                                                           mManager->mEntities[i].mCollider.mBoxSize);
        }
    }
}
