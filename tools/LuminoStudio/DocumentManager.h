#pragma once
#include <QMainWindow>
#include "SceneListDock.h"

class DocumentManager : public QObject
{
    Q_OBJECT
public:
    DocumentManager(QObject* parent = nullptr);
    ~DocumentManager();

    QWidget* rootWidget() const { return m_rootWidget; }

private:
    QWidget* m_rootWidget;
    QTabWidget* m_tabWidget;
};
