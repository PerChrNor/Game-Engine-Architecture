#include "boxcolliderwidget.h"
#include "ui_boxcolliderwidget.h"

BoxColliderWidget::BoxColliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxColliderWidget)
{
    ui->setupUi(this);

}

BoxColliderWidget::~BoxColliderWidget()
{
    delete ui;
}

void BoxColliderWidget::SetBoxSize(Vector3D A)
{
    if(A.x >= 0 && A.y >= 0 && A.z >= 0){

      Vector3D temp;
      Vector3D temp2;
      //set the size of the box collider
      temp.x = mCurrentManager->mEntities[indexInEntites].mCollider.mCenter.x() + A.x;
      temp.y = mCurrentManager->mEntities[indexInEntites].mCollider.mCenter.y() + A.y;
      temp.z = mCurrentManager->mEntities[indexInEntites].mCollider.mCenter.z() + A.z;

      temp2.x = mCurrentManager->mEntities[indexInEntites].mCollider.mCenter.x() + -A.x;
      temp2.y = mCurrentManager->mEntities[indexInEntites].mCollider.mCenter.y() + -A.y;
      temp2.z = mCurrentManager->mEntities[indexInEntites].mCollider.mCenter.z() + -A.z;

      mCurrentManager->mEntities[indexInEntites].mCollider.mMaxPoint = temp;
      mCurrentManager->mEntities[indexInEntites].mCollider.mMinPoint = temp2;
    }

}

void BoxColliderWidget::init()
{
    GetBoxSize();
}


void BoxColliderWidget::on_SizeY_valueChanged(double arg1)
{
    YValue = arg1;
    XYZValue = Vector3D(XValue,YValue,ZValue);
    SetBoxSize(XYZValue);
}

void BoxColliderWidget::on_SizeZ_valueChanged(double arg1)
{
    ZValue = arg1;
    XYZValue = Vector3D(XValue,YValue,ZValue);
    SetBoxSize(XYZValue);
}

void BoxColliderWidget::GetBoxSize()
{
    XValue = mCurrentManager->mEntities[indexInEntites].mCollider.mBoxSize.x;
    YValue = mCurrentManager->mEntities[indexInEntites].mCollider.mBoxSize.y;
    ZValue = mCurrentManager->mEntities[indexInEntites].mCollider.mBoxSize.z;


    ui->SizeX->setValue(XValue);
    ui->SizeY->setValue(YValue);
    ui->SizeZ->setValue(ZValue);
}

void BoxColliderWidget::on_SizeX_valueChanged(double arg1)
{
    XValue = arg1;
    XYZValue = Vector3D(XValue,YValue,ZValue);
    SetBoxSize(XYZValue);
}
