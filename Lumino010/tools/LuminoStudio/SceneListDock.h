#ifndef SCENELISTDOCKWIDGET_H
#define SCENELISTDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class SceneListDock;
}

class SceneListDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit SceneListDock(QWidget *parent = 0);
    ~SceneListDock();

private:
    Ui::SceneListDock *ui;
};

#endif // SCENELISTDOCKWIDGET_H
