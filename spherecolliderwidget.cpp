#include "spherecolliderwidget.h"
#include "ui_spherecolliderwidget.h"

SphereColliderWidget::SphereColliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SphereColliderWidget)
{
    ui->setupUi(this);
}

SphereColliderWidget::~SphereColliderWidget()
{
    delete ui;
}

void SphereColliderWidget::init()
{
    getSphereSize();
}

void SphereColliderWidget::getSphereSize()
{
    mRadius = mCurrentManager->mEntities[indexInEntites].mCollider.mRadius;
    ui->Radius->setValue(mRadius);
}

void SphereColliderWidget::on_Radius_valueChanged(double arg1)
{
    mRadius = arg1;
    mCurrentManager->mEntities[indexInEntites].mCollider.mRadius = mRadius;
}
