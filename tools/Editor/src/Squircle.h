
#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

#pragma comment(lib, "Opengl32.lib")

//! [1]
class Squircle : public QQuickItem
{
    Q_OBJECT

        Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    Squircle();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void paint();
    void cleanup();
    void sync();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QOpenGLShaderProgram *m_program;

    qreal m_t;
    qreal m_thread_t;

    QSize m_viewportSize;
};
//! [1]





#include <QtQuick/QQuickFramebufferObject>

class LogoRenderer;

class FboInSGRenderer : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    //FboInSGRenderer()
    //{
    //    connect(this, SIGNAL(sceneGraphInitialized()), this, SLOT(onSceneGraphInitialized()), Qt::DirectConnection);
    //}

    Renderer *createRenderer() const;


};





#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>
#include <QOpenGLFunctions>

#include <QTime>
#include <QVector>

class LogoRenderer : protected QOpenGLFunctions
{
public:
    LogoRenderer();
    ~LogoRenderer();

    void render();
    void initialize();

private:

    qreal   m_fAngle;
    qreal   m_fScale;

    void paintQtLogo();
    void createGeometry();
    void quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4);
    void extrude(qreal x1, qreal y1, qreal x2, qreal y2);

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QOpenGLShaderProgram program1;
    int vertexAttr1;
    int normalAttr1;
    int matrixUniform1;
};




#endif // SQUIRCLE_H