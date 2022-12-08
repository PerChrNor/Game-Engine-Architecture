#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>

#include "renderwindow.h"
#include "Math/matrix4x4.h"
#include "coreengine.h"
#include "scene.h"
#include "widgettransform.h"
#include "Shaders/shader.h"
#include "Shaders/plainshader.h"
#include "Shaders/phongshader.h"
#include "Shaders/textureshader.h"
#include "collisionwidget.h"
#include "boxcolliderwidget.h"
#include "spherecolliderwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this - sorry!)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types than OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have some OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Just prints out what OpenGL format we try to get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();
}
/// Updates the entity outliner
///
/// This function inserts all the entities in a treewidget where they are represented by the name they are given when they are made.
/// Written by candidate 8008.
/// @param object The vector of entities where each entity is held.
void MainWindow::updateUI(const std::vector<class Entity> &object)
{
    ui->treeWidget->clear();

    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0, QString::fromStdString(mRenderWindow->mScene1->mName));
    ui->treeWidget->addTopLevelItem(root);
    ui->treeWidget->expandAll();

    for (unsigned i = 0; i < object.size(); ++i)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(root);
        item->setText(0, QString::fromStdString(object.at(i).mName));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}
/// Used for selecting objects in outliner when using mousepicking.
/// Written by candidate 8008
/// @param name The entity name of the entity that is mouse picked.
void MainWindow::selectObjectInOutliner(QString name)
{
    if (name == "" || name == "Scene 1")
        on_treeWidget_itemClicked(nullptr, 0);
    else
    {
        QList<QTreeWidgetItem *> list = ui->treeWidget->findItems(name, Qt::MatchFixedString|Qt::MatchRecursive);
        on_treeWidget_itemClicked(list.first(),0);
    }
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_wireframe_clicked()
{
    mRenderWindow->wireFrame = !mRenderWindow->wireFrame;
    qDebug() << "wireframe button clicked \n";
    mRenderWindow->changeRender();
}

void MainWindow::on_pushButton_play_clicked()
{
    mRenderWindow->editor = !mRenderWindow->editor;
    if(mRenderWindow->editor)
    {
        ui->pushButton_play->setText("Play");
        mRenderWindow->mManager->stopAllSound();
        static_cast<PlainShader*>(ResourceManager::getInstance().mShaderProgram[PLAINSHADER])->CurrentCamera = mRenderWindow->mCamera;
        static_cast<TextureShader*>(ResourceManager::getInstance().mShaderProgram[TEXTURESHADER])->CurrentCamera = mRenderWindow->mCamera;
        static_cast<PhongShader*>(ResourceManager::getInstance().mShaderProgram[PHONGSHADER])->CurrentCamera = mRenderWindow->mCamera;
        static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[MOUSEPICKERSHADER])->CurrentCamera = mRenderWindow->mCamera;
        //mRenderWindow->mManager->mEntities[0].mTransform.position = QVector3D(0.f, 1.0f, 0.f);
        //mRenderWindow->mManager->mEntities[0].mTransform.rotation = QVector3D(0.f, 180.f, 0.f);
    }
    else
    {
        ui->pushButton_play->setText("Stop");
        mRenderWindow->mManager->startAllSound();
        static_cast<PlainShader*>(ResourceManager::getInstance().mShaderProgram[PLAINSHADER])->CurrentCamera = mRenderWindow->mGameCamera;
        static_cast<TextureShader*>(ResourceManager::getInstance().mShaderProgram[TEXTURESHADER])->CurrentCamera = mRenderWindow->mGameCamera;
        static_cast<PhongShader*>(ResourceManager::getInstance().mShaderProgram[PHONGSHADER])->CurrentCamera = mRenderWindow->mGameCamera;
        static_cast<MousePickerShader*>(ResourceManager::getInstance().mShaderProgram[MOUSEPICKERSHADER])->CurrentCamera = mRenderWindow->mGameCamera;
    }

}

void MainWindow::on_pushButton_xyz_clicked()
{
    mRenderWindow->drawXYZ = !mRenderWindow->drawXYZ;
}

void MainWindow::on_pushButton_object_clicked()
{
   mRenderWindow->SpawnNewObject();
}

void MainWindow::on_pushButton_mousepicker_clicked()
{
    renderMousePick = !renderMousePick;
    qDebug() << "Mousepick" << renderMousePick;
}
/// The function which activates when clicking the name of an object in the outliner.
///
/// When an item is clicked in the outliner or through mouse picking this function creates the needed widgets and reads the needed variables into the widgets.
/// Written by candidate 8008 and 8002
/// @param item The entity in the treeWidget which is clicked.
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int)
{
    clearLayout(ui->gridLayout_2);

    if (!item || item->text(0) == mRenderWindow->mScene1->mName.c_str())
    {
        ui->lineEditName->setText("no selection");
        return;
    }
    mCurrentItem = item;
    //ui->lineEditName->setText(mCurrentItem->text(0));
    item->setSelected(true);

    //Transform widget
    mTransformWidget = new WidgetTransform(this);
    mTransformWidget->setObjectName("TransformWidget");
    ui->gridLayout_2->addWidget(mTransformWidget);
    mCurrentItemIndex = item->parent()->indexOfChild(item);
    mTransformWidget->indexInEntites = mCurrentItemIndex;
    mTransformWidget->mCurrentManager = mRenderWindow->mManager;
    mTransformWidget->readPos();
    mTransformWidget->readRot();
    mTransformWidget->readScale();
    mTransformWidget->clearFocus();

    //Collision widget
    mCollisionWidget = new CollisionWidget(this);
    mCollisionWidget->setObjectName("CollisionWidget");
    ui->gridLayout_2->addWidget(mCollisionWidget);
    mCollisionWidget->indexInEntites = mCurrentItemIndex;
    mCollisionWidget->mCurrentManager = mRenderWindow->mManager;
    mCollisionWidget->SetTypeShape();



    if(mRenderWindow->mManager->mEntities[mCurrentItemIndex].mCollider.mShape == "Box"){
    mBoxCollisionWidget = new BoxColliderWidget(this);
    mBoxCollisionWidget->setObjectName("boxWidget");
    ui->gridLayout_2->addWidget(mBoxCollisionWidget);
    mBoxCollisionWidget->indexInEntites = mCurrentItemIndex;
    mBoxCollisionWidget->mCurrentManager = mRenderWindow->mManager;
    mBoxCollisionWidget->init();


    }

    else if(mRenderWindow->mManager->mEntities[mCurrentItemIndex].mCollider.mShape == "Sphere"){
        mSphereCollisionWidget = new SphereColliderWidget(this);
        mSphereCollisionWidget->setObjectName("sphereWidget");
        ui->gridLayout_2->addWidget(mSphereCollisionWidget);
        mSphereCollisionWidget->indexInEntites = mCurrentItemIndex;
        mSphereCollisionWidget->mCurrentManager = mRenderWindow->mManager;
        mSphereCollisionWidget->init();
    }




}

void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        if (item->widget())
            delete item->widget();
        delete item;
    }
    mTransformWidget = nullptr;
    ui->treeWidget->clearSelection();
}

void MainWindow::on_pushButton_runscript_clicked()
{
    mScript.runScript();
    mRenderWindow->mCarSpeed = mScript.carSpeed;
}
























