#pragma once
#include "Document.h"

class SceneEditorDocument : public Document
{
	Q_OBJECT
public:
    SceneEditorDocument(QObject* parent = nullptr);
};



class SceneEditorView : public QWidget
{
    Q_OBJECT
public:
    SceneEditorView(QWidget* parent = nullptr);

private:
};
