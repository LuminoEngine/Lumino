#include "SceneListDock.h"
#include "ui_SceneListDock.h"

SceneListDock::SceneListDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SceneListDock)
{
    ui->setupUi(this);
}

SceneListDock::~SceneListDock()
{
    delete ui;
}
