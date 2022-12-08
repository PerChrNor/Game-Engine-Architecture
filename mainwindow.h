#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scriptingsystem.h"



class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    class CoreEngine *mCoreEngine {nullptr};

    void updateUI(const std::vector<class Entity> &object);

    void selectObjectInOutliner(QString name);

    class WidgetTransform* mTransformWidget{nullptr};
    bool renderMousePick{false};
    QWidget *mRenderWindowContainer;

    bool isRunning{false};


    class CollisionWidget * mCollisionWidget{nullptr};

    class BoxColliderWidget * mBoxCollisionWidget{nullptr};

    class SphereColliderWidget * mSphereCollisionWidget{nullptr};




private slots:
    void on_pushButton_wireframe_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_xyz_clicked();

    void on_pushButton_object_clicked();

    void on_treeWidget_itemClicked(class QTreeWidgetItem *item, int column);

    void on_pushButton_mousepicker_clicked();

    void on_pushButton_runscript_clicked();

private:
    void init();
    Ui::MainWindow *ui;
    RenderWindow *mRenderWindow{nullptr};

    //RenderWindow *mRenderWindow;
    QTreeWidgetItem *mCurrentItem{nullptr};
    int mCurrentItemIndex{-1};
    void clearLayout(QLayout *layout);

    bool arg1{false};

    ScriptingSystem mScript;



};

#endif // MAINWINDOW_H
