#ifndef WIDGETTRANSFORM_H
#define WIDGETTRANSFORM_H

#include <QWidget>
#include "Math/vector3d.h"
#include "resourcemanager.h"

namespace Ui {
class WidgetTransform;
}
/// Makes a widget which shows the transform of an entity and allows the user to change it.
/// Written by candidate 8008
class WidgetTransform : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTransform(QWidget *parent = nullptr);
    ~WidgetTransform();

    void readPos();
    void readRot();
    void readScale();

    int indexInEntites{-1};
    ResourceManager* mCurrentManager{nullptr};

    float posStep{1.f};
    float rotStep{1.f};
    float scaleStep{0.1f};

private slots:
    void on_doubleSpinBoxPosX_valueChanged(double arg1);

    void on_doubleSpinBoxPosY_valueChanged(double arg1);

    void on_doubleSpinBoxPosZ_valueChanged(double arg1);

    void on_doubleSpinBoxRotX_valueChanged(double arg1);

    void on_doubleSpinBoxRotY_valueChanged(double arg1);

    void on_doubleSpinBoxRotZ_valueChanged(double arg1);

    void on_doubleSpinBoxScaleX_valueChanged(double arg1);

    void on_doubleSpinBoxScaleY_valueChanged(double arg1);

    void on_doubleSpinBoxScaleZ_valueChanged(double arg1);

private:
    Ui::WidgetTransform *ui;
};

#endif // WIDGETTRANSFORM_H
