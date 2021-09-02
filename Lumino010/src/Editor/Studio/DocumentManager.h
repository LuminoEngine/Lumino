#pragma once
#include <QMainWindow>
#include "Document.h"

class StartupView;

class DocumentManager : public QObject
{
    Q_OBJECT
public:
    DocumentManager(QWidget* parent = nullptr);
    ~DocumentManager();

	void addDocument(EditorDocument* doc);

    QWidget* rootWidget() const { return m_rootWidget; }

private:
    QWidget* m_rootWidget;
	QStackedLayout* m_rootLayout;
	QList<EditorDocument*> m_documents;
    //QTabWidget* m_tabWidget;
    QTabBar* m_tabbar;
	QStackedLayout* m_viewStack;
	StartupView* m_startupView;
};
