#ifndef OPENGL_H
#define OPENGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QDebug>
#include <QTimer>

class OpenGL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGL(QWidget *parent = nullptr);

    void initializeGL() override;
    void render();

public slots:
    void animate();
    void changeChooseVal();

protected:
    void paintGL() override;
    //void paintEvent(QPaintEvent *event) override;
    void renderTriangle(const GLfloat vertices[], const GLfloat colors[]);
    void renderLines(const GLfloat vertices[], const GLfloat colors[]);
    void renderQuad(const GLfloat vertices[], const GLfloat colors[]);

private:
    QOpenGLShaderProgram *shaderProgram = nullptr;
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    int m_frame = 0;
    char chooseVal = 0;
};
#endif // OPENGL_H
