#pragma once
#include "ContentsViewManager.h"


class SceneAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit SceneAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class SceneAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SceneAssetTreeView(QWidget* parent = nullptr);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    SceneAssetTreeModel* m_model;
};


class SceneContentsViewProvider : public ContentsViewProvider
{
public:
    SceneContentsViewProvider(QObject* parent);
    virtual QString icon() const override { return "windowrestore"; }
    virtual QWidget* createView() override;

private:
    QWidget* m_rootWidget;
};

