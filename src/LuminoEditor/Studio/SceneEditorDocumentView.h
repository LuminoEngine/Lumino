#pragma once
#include "Document.h"

class LuminoWidget;

class SceneEditorDocument : public EditorDocument
{
	Q_OBJECT
public:
    SceneEditorDocument(QObject* parent = nullptr);
};



class SceneEditorView : public EditorView
{
    Q_OBJECT
public:
    SceneEditorView(QWidget* parent = nullptr);

private:
    QVBoxLayout* m_layout1;
    LuminoWidget* m_luminoWidget;
};
