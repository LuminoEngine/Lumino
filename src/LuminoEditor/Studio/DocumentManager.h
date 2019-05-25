#pragma once
#include <QMainWindow>
//#include "SceneListDock.h"

class Document : public QObject
{
	Q_OBJECT
public:
	Document(QObject* parent = nullptr);
};



class DocumentManager : public QObject
{
    Q_OBJECT
public:
    DocumentManager(QWidget* parent = nullptr);
    ~DocumentManager();

	void addDocument(Document* doc);

    QWidget* rootWidget() const { return m_rootWidget; }

private:
    QWidget* m_rootWidget;
	QStackedLayout* m_rootLayout;
	QList<Document*> m_documents;
    QTabWidget* m_tabWidget;
};
