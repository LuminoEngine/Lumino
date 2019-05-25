#pragma once
#include "ContentsViewManager.h"


class AudioAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit AudioAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class AudioAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit AudioAssetTreeView(QWidget* parent = nullptr);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    AudioAssetTreeModel* m_model;
};



class AudioContentsViewProvider : public ContentsViewProvider
{
public:
    virtual ln::String icon() const override { return u"music"; }
    virtual QWidget* createView() override;

LN_CONSTRUCT_ACCESS:
    AudioContentsViewProvider();

private:
    AudioAssetTreeView* m_treeView;
};

