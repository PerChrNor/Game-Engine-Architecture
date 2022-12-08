#ifndef BOXCOLLIDERWIDGET_H
#define BOXCOLLIDERWIDGET_H

#include <QWidget>
#include "resourcemanager.h"

namespace Ui {
class BoxColliderWidget;
}
///
/// \brief The BoxColliderWidget class is in charge of changing the BoxCollider's size when program is running.
///
class BoxColliderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoxColliderWidget(QWidget *parent = nullptr);
    ~BoxColliderWidget();

    ResourceManager* mCurrentManager{nullptr};

    int indexInEntites{-1};
    ///
    /// \brief SetBoxSize sets the box colliders size.
    /// \param BoxSize the value changed in the editor
    ///
    void SetBoxSize(Vector3D BoxSize);

    /// \brief initatilizes the function.
    void init();

private slots:


    void on_SizeY_valueChanged(double arg1);

    void on_SizeZ_valueChanged(double arg1);

    void on_SizeX_valueChanged(double arg1);

private:
    Ui::BoxColliderWidget *ui;

    float XValue = 0;
    float YValue = 0;
    float ZValue = 0;
    Vector3D XYZValue;

    ///
    /// \brief GetBoxSize gets the box colliders size.
    ///
    void GetBoxSize();

};

#endif // BOXCOLLIDERWIDGET_H
