#ifndef COLLISIONWIDGET_H
#define COLLISIONWIDGET_H

#include <QWidget>
#include "resourcemanager.h"

namespace Ui {
class CollisionWidget;
}
///
/// \brief The CollisionWidget class when clicking on an object, this is created to show collider information.
///
class CollisionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CollisionWidget(QWidget *parent = nullptr);
    ~CollisionWidget();
    ///
    /// \brief SetTypeShape changes text in the widget to the correct Type and Shape of the selected object.
    ///
    void SetTypeShape();

    ResourceManager* mCurrentManager{nullptr};

    int indexInEntites{-1};

private:
    Ui::CollisionWidget *ui;
};

#endif // COLLISIONWIDGET_H
