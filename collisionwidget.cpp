#include "collisionwidget.h"
#include "ui_collisionwidget.h"

CollisionWidget::CollisionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CollisionWidget)
{
    ui->setupUi(this);
}

CollisionWidget::~CollisionWidget()
{
    delete ui;
}

void CollisionWidget::SetTypeShape()
{
    std::string shape = mCurrentManager->mEntities[indexInEntites].mCollider.mShape;
    ui->Shape->setText(QString::fromStdString(shape));
    std::string type = mCurrentManager->mEntities[indexInEntites].mCollider.mType;
    ui->Type->setText(QString::fromStdString(type));
}
