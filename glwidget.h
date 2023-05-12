#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <algorithm>
#include <QtMath>
#include <QRandomGenerator>
#include <QPainterPath>


class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
   ~GLWidget();

    void paintRectDemo(QPainter* painter);
    void paintPatchDemo(QPainter *painter);
    void paintData(QPainter* painter, const QVector<QVector<char>>* colorDataVec);

public slots:
    void animate();
    void changeChooseVal();

protected:
    void paintGL() override;
    //paintRect()
    void paintRectLines(QPainter* painter);
    void paintRectFill(QPainter* painter);
    void paintRectTexture(QPainter* painter);
    void paintText(QPainter *painter, const size_t &angle);
    //paintData()
    void paintDataHelper(QPainter* painter, const QVector<char> colorVec, double paintAngle);
    void generateRandomPoints(QVector<QVector<double>>* vec);
    void transformDeviceDataVecToColorDataVec(const QVector<QVector<double>>* deviceDataVec, QVector<QVector<char>>* colorDataVec);
    QVector<char> transformDeviceDataToColorData(const QVector<double> &deviceData);
    char transformToColor(const double& val);

    void generateRandomPointsVal(QVector<QVector<char>>* vec);

private:
    QBrush brush;
    QFont textFont;
    QPen pen;
    QPen textPen;
    size_t angle = 0;
    char chooseVal = 0;

    int pointsResolution = 150;
    int linesResolution = 100;
    QVector<QVector<double>>* deviceDataVec;
    QVector<QVector<char>>* colorDataVec;
};
#endif // GLWIDGET_H
