#pragma once
#include "Document.h"

class LuminoWidget;

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
    QVBoxLayout* m_layout1;
    LuminoWidget* m_luminoWidget;
};
