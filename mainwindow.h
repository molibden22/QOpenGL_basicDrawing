#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QDebug>

#include "glwidget.h"
#include "opengl.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void glButtonClicked() const;
    void openGLButtonClicked() const;

private:
    GLWidget* glwidget;
    OpenGL* openGL;

};
#endif // MAINWINDOW_H
