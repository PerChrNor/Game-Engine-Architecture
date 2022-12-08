#include "widgettransform.h"
#include "ui_widgettransform.h"

WidgetTransform::WidgetTransform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTransform)
{

    ui->setupUi(this);
    ui->doubleSpinBoxPosX->setMinimum(-1000);
    ui->doubleSpinBoxPosY->setMinimum(-1000);
    ui->doubleSpinBoxPosZ->setMinimum(-1000);

    ui->doubleSpinBoxRotX->setMinimum(-360);
    ui->doubleSpinBoxRotY->setMinimum(-360);
    ui->doubleSpinBoxRotZ->setMinimum(-360);

    ui->doubleSpinBoxRotX->setMaximum(360);
    ui->doubleSpinBoxRotY->setMaximum(360);
    ui->doubleSpinBoxRotZ->setMaximum(360);

    ui->doubleSpinBoxScaleX->setMinimum(-1000);
    ui->doubleSpinBoxScaleY->setMinimum(-1000);
    ui->doubleSpinBoxScaleZ->setMinimum(-1000);
}

WidgetTransform::~WidgetTransform()
{
    delete ui;
}

void WidgetTransform::readPos()
{
    QVector3D pos = mCurrentManager->mEntities[indexInEntites].mTransform.position;
    ui->doubleSpinBoxPosX->setValue(pos.x());
    ui->doubleSpinBoxPosY->setValue(pos.y());
    ui->doubleSpinBoxPosZ->setValue(pos.z());
}

void WidgetTransform::readRot()
{
    QVector3D rot = mCurrentManager->mEntities[indexInEntites].mTransform.rotation;
    ui->doubleSpinBoxRotX->setValue(rot.x());
    ui->doubleSpinBoxRotY->setValue(rot.y());
    ui->doubleSpinBoxRotZ->setValue(rot.z());
}

void WidgetTransform::readScale()
{
    QVector3D scale = mCurrentManager->mEntities[indexInEntites].mTransform.scale;
    ui->doubleSpinBoxScaleX->setValue(scale.x());
    ui->doubleSpinBoxScaleY->setValue(scale.y());
    ui->doubleSpinBoxScaleZ->setValue(scale.z());
}

/// Changes the x position for the entity
void WidgetTransform::on_doubleSpinBoxPosX_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.position.setX(arg1);
    mCurrentManager->updateSoundPos(indexInEntites);
    mCurrentManager->updateColliderPos(indexInEntites);
}

/// Changes the y position for the entity
void WidgetTransform::on_doubleSpinBoxPosY_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.position.setY(arg1);
    mCurrentManager->updateSoundPos(indexInEntites);
    mCurrentManager->updateColliderPos(indexInEntites);

}

/// Changes the z position for the entity
void WidgetTransform::on_doubleSpinBoxPosZ_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.position.setZ(arg1);
    mCurrentManager->updateSoundPos(indexInEntites);
    mCurrentManager->updateColliderPos(indexInEntites);
}

/// Changes the x rotation for the entity
void WidgetTransform::on_doubleSpinBoxRotX_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.rotation.setX(arg1);
}

/// Changes the y rotation for the entity
void WidgetTransform::on_doubleSpinBoxRotY_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.rotation.setY(arg1);
}

/// Changes the z rotation for the entity
void WidgetTransform::on_doubleSpinBoxRotZ_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.rotation.setZ(arg1);
}

/// Changes the x scale for the entity
void WidgetTransform::on_doubleSpinBoxScaleX_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.scale.setX(arg1);
}

/// Changes the y scale for the entity
void WidgetTransform::on_doubleSpinBoxScaleY_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.scale.setY(arg1);
}

/// Changes the z scale for the entity
void WidgetTransform::on_doubleSpinBoxScaleZ_valueChanged(double arg1)
{
    mCurrentManager->mEntities[indexInEntites].mTransform.scale.setZ(arg1);
}
