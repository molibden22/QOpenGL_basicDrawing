#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    glwidget(new GLWidget(this)),
    openGL(new OpenGL(this))
{
    setWindowTitle(tr("2D Painting on Widgets"));
    setGeometry(QRect(400, 300, 800, 600));

    QPushButton *glButton = new QPushButton(">glwidget<", this);
    QPushButton *openGLButton = new QPushButton(">openGL<", this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *glwidgetlayout = new QVBoxLayout();
    QVBoxLayout *openGLlayout = new QVBoxLayout();

    mainLayout->addLayout(glwidgetlayout);
    glwidgetlayout->addWidget(glwidget, 9);
    glwidgetlayout->addWidget(glButton,1);

    mainLayout->addLayout(openGLlayout);
    openGLlayout->addWidget(openGL, 9);
    openGLlayout->addWidget(openGLButton, 1);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, glwidget, &GLWidget::animate);
    connect(timer, &QTimer::timeout, openGL, &OpenGL::animate);
    timer->start(15);

    connect(glButton, &QPushButton::clicked, this, &MainWindow::glButtonClicked);
    connect(openGLButton, &QPushButton::clicked, this, &MainWindow::openGLButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::glButtonClicked() const { glwidget->changeChooseVal(); }
void MainWindow::openGLButtonClicked() const { openGL->changeChooseVal();  }
