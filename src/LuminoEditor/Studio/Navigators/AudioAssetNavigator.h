#pragma once
#include "../ContentsViewManager.h"


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

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    AudioAssetTreeModel* m_model;
};



class AudioContentsViewProvider : public ContentsViewProvider
{
public:
    AudioContentsViewProvider(QObject* parent);
    virtual QString icon() const override { return "music"; }
    virtual QWidget* createView() override;
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    AudioAssetTreeView* view() const { return m_treeView; }

private:
    AudioAssetTreeView* m_treeView;
};

