#pragma once

class ContentsViewProvider;

class ContentsViewManager : public QObject
{
    Q_OBJECT
public:
    ContentsViewManager();
    QFrame* sidebar() { return m_sidear; }

    void addContentsViewProvider(ContentsViewProvider* value);

    

protected:

private:
    QFrame* m_sidear;
    ln::List<ln::Ref<ContentsViewProvider>> m_providers;
};


class ContentsViewProvider : public ln::Object
{
public:
    virtual ln::String iconResource() const = 0;

LN_CONSTRUCT_ACCESS:
    ContentsViewProvider();

private:
};



class AudioContentsViewProvider : public ContentsViewProvider
{
public:

LN_CONSTRUCT_ACCESS:
    AudioContentsViewProvider();

private:
};

