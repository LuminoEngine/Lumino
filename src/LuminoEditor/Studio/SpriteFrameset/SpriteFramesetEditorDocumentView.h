#pragma once
#include "Document.h"

class LuminoWidget;

class SpriteFramesetEditorDocument : public EditorDocument
{
	Q_OBJECT
public:
    SpriteFramesetEditorDocument(QObject* parent = nullptr);
};



class SpriteFramesetEditorView : public QWidget
{
    Q_OBJECT
public:
    SpriteFramesetEditorView(QWidget* parent = nullptr);

private:
    QVBoxLayout* m_layout1;
    LuminoWidget* m_luminoWidget;
};
