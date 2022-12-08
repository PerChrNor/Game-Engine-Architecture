#ifndef SPHERECOLLIDERWIDGET_H
#define SPHERECOLLIDERWIDGET_H

#include <QWidget>
#include "resourcemanager.h"

namespace Ui {
class SphereColliderWidget;
}
///
/// \brief The SphereColliderWidget class is in charge of changing the SphereCollider's size when program is running.
///
class SphereColliderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SphereColliderWidget(QWidget *parent = nullptr);
    ~SphereColliderWidget();

    ResourceManager* mCurrentManager{nullptr};

    int indexInEntites{-1};

    ///
    /// \brief initatilizes the function.
    ///
    void init();

private slots:
    void on_Radius_valueChanged(double arg1);

private:
    Ui::SphereColliderWidget *ui;

    float mRadius;

    ///
    /// \brief GetSphereSize gets the sphere colliders radius.
    ///
    void getSphereSize();
};

#endif // SPHERECOLLIDERWIDGET_H
