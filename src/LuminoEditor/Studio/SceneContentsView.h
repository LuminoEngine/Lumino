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
    virtual ln::String icon() const override { return u"windowrestore"; }
    virtual QWidget* createView() override;

LN_CONSTRUCT_ACCESS:
    SceneContentsViewProvider();

private:
    QWidget* m_rootWidget;
};

