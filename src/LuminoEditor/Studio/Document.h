#pragma once

class EditorDocument : public QObject
{
	Q_OBJECT
public:
    EditorDocument(QObject* parent = nullptr);
};

class EditorView : public QFrame
{
    Q_OBJECT
public:
    EditorView(QWidget* parent = nullptr);
};
