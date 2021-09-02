#include "SceneListDock.h"
#include "ui_SceneListDock.h"

#include <QListWidget>

SceneListDock::SceneListDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SceneListDock)
{
    ui->setupUi(this);


	QListWidget* listWidget = new QListWidget(this);
	listWidget->addItem("test1");
	listWidget->addItem("test2");

	QListWidgetItem *item = new QListWidgetItem();
	item->setSizeHint(QSize(50, 50));
	item->setBackground(QBrush(QColor(255, 0, 0)));
	listWidget->addItem(item);

	setWidget(listWidget);
}

SceneListDock::~SceneListDock()
{
    delete ui;
}
