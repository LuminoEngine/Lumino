#pragma once
#include <LuminoEditor/Plugin.hpp>

class LuminoWidget;

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

class AssetEditorDocument : public EditorDocument
{
    Q_OBJECT
public:
    AssetEditorDocument(ln::AssetModel* asset, ln::AssetEditorViewModel* editor, QObject* parent = nullptr);
    const Ref<ln::AssetModel>& asset() const { return m_asset; }
    const Ref<ln::AssetEditorViewModel>& editor() const { return m_editor; }

private:
    Ref<ln::AssetModel> m_asset;
    Ref<ln::AssetEditorViewModel> m_editor;
};

class AssetEditorView : public EditorView
{
    Q_OBJECT
public:
    AssetEditorView(AssetEditorDocument* document, QWidget* parent = nullptr);

private:
    AssetEditorDocument* m_document;
    QVBoxLayout* m_rootLayout;
    LuminoWidget* m_luminoWidget;
};