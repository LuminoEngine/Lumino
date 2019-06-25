#pragma once
#include "../ContentsViewManager.h"
#include "../Widgets/Expander.h"

class SpritesetAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit SpritesetAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class SpritesetAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SpritesetAssetTreeView(QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex& index);

private:
    SpritesetAssetTreeModel* m_model;
};



class SpritesetContentsViewProvider : public ContentsViewProvider
{
public:
    SpritesetContentsViewProvider(lna::Project* project, QWidget* parent);
    virtual QString icon() const override { return "fileimageo"; }
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    SpritesetAssetTreeView* view() const { return m_treeView; }

private:
	QVBoxLayout* m_rootLayout;
    SpritesetAssetTreeView* m_treeView;
	QPushButton* m_newFileButton;
	QPushButton* m_newFolderButton;
	Expander* m_expander;
};

