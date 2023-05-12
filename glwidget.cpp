#include "glwidget.h"
#include <QRect>

QLinearGradient setGradient()
{
    QLinearGradient gradient
    (QPointF(50,-20), QPointF(120,120));
    gradient.setColorAt(0.0, QColor(0xFF, 0xFF, 0xFF, 0x33));
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39, 0x33));
    return gradient;
}

QPainterPath setPath(QPoint start, int xRes, int yRes)
{
    QPainterPath path;
    path.moveTo(start);
    QRect bigRect(start.x() + xRes * 0.75, start.y() - 0.3 * yRes, xRes * 0.25, yRes);
    QRect smallRect(xRes * (-0.05), start.y() - 0.10 * yRes, xRes * 0.25, yRes * 0.60);
    path.arcTo(bigRect, 60, -yRes * 0.25);
    path.arcTo(smallRect, -40, yRes * 0.16);
    return path;
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(500, 500);
    setAutoFillBackground(false);

    auto gradient = setGradient();
    brush = QBrush(gradient);
    pen.setColor(Qt::white);
    pen.setWidth(1);
    textPen.setColor(Qt::white);
    textFont.setPixelSize(80);

    deviceDataVec = new QVector<QVector<double>>;
    generateRandomPoints(deviceDataVec);
    colorDataVec = new QVector<QVector<char>>;
    transformDeviceDataVecToColorDataVec(deviceDataVec, colorDataVec);
}

GLWidget::~GLWidget()
{
    delete deviceDataVec;
    delete colorDataVec;
}

void GLWidget::paintRectDemo(QPainter* painter)
{
    painter->translate(this->rect().width()/2,this->rect().height()/2);
    painter->rotate(angle * 0.120);

    if (angle < 4000)
        paintRectLines(painter);
    else if (angle > 8000)
        paintRectTexture(painter);
    else
        paintRectFill(painter);
    paintText(painter, angle);
}

void GLWidget::paintPatchDemo(QPainter *painter)
{
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->setBrush(QBrush(QPixmap(":/src/texture.png")));

    int xResolution = 400;
    int yResolution = 500;
    QPoint start(0.167*xResolution,0.30*yResolution);
    QPainterPath path = setPath(start, xResolution, yResolution);
    painter->drawPath(path);
}

void GLWidget::paintData(QPainter* painter, const QVector<QVector<char>>* colorDataVec)
{
    double startAngle = -linesResolution / 2 * 0.008f;
    size_t counter = 0;
    for(const auto& vec : *colorDataVec){
        auto paintAngle = startAngle + counter * 0.008f;
        paintDataHelper(painter, vec, paintAngle);
        counter++;
    }
}

void GLWidget::animate()
{
    angle = (angle + qobject_cast<QTimer*>(sender())->interval()) % 12000 ;
    update();
}

void GLWidget::changeChooseVal()
{
    chooseVal += 1;
    if (chooseVal > 2)
        chooseVal = 0;
}

void GLWidget::paintGL()
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch( chooseVal)
    {
    case 0:
        paintRectDemo(&painter);
        break;
    case 1:
        paintPatchDemo(&painter);
        break;
    case 2:
        paintData(&painter, colorDataVec);
        break;
    default:
        break;
    }
    painter.end();
}

void GLWidget::paintRectLines(QPainter* painter)
{
    painter->setPen(pen);
    painter->drawRect(QRectF(-150, -150, 300, 300));
}

void GLWidget::paintRectFill(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);
    painter->drawRect(QRectF(-150, -150, 300, 300));
}

void GLWidget::paintRectTexture(QPainter *painter)
{
    painter->setPen(pen);
    painter->setBrush(QBrush(QPixmap(":/src/texture.png")));
    painter->drawRect(QRectF(-150, -150, 300, 300));
}

void GLWidget::paintText(QPainter *painter, const size_t &angle)
{
    painter->setPen(QPen(QColor((angle)%255,(2*angle)%255,(5*angle)%255,255)));
    textFont.setPixelSize(80);
    painter->setFont(textFont);
    painter->drawText(10,10,"Qt");
}

void GLWidget::paintDataHelper(QPainter* painter, const QVector<char> colorVec, double paintAngle)
{
    QPen penPoint(Qt::white);
    penPoint.setWidth(3);

    size_t counter = 0;
    QPoint startPoint(this->rect().width() / 2, -70);
    for(const auto& val : colorVec){
        auto colorVal = std::max(val * 45, 20);
        penPoint.setColor(QColor(colorVal, colorVal, colorVal, 255));
        painter->setPen(penPoint);

        int rl = -startPoint.y() * 2 + counter;
        int ry = std::cos(paintAngle) * rl;
        int rx = std::sin(paintAngle) * rl;

        painter->drawPoint(startPoint.x()+rx, startPoint.y()+ry);
        counter += 3;
    }
}

void GLWidget::generateRandomPoints(QVector<QVector<double>> *vec) {
    QVector<double> temp(pointsResolution);
    for(size_t i = 0 ; i < linesResolution ; ++i){
        std::generate(temp.begin(), temp.end(), [](){return QRandomGenerator::global()->generateDouble();});
        vec->push_back(temp);
    }
}

void GLWidget::transformDeviceDataVecToColorDataVec(const QVector<QVector<double>>* deviceDataVec, QVector<QVector<char>>* colorDataVec)
{
    std::transform(deviceDataVec->cbegin(), deviceDataVec->cend(), std::back_inserter(*colorDataVec),
                   [this](const QVector<double>& vec) { return transformDeviceDataToColorData(vec); });
}

QVector<char> GLWidget::transformDeviceDataToColorData(const QVector<double> &deviceData)
{
    QVector<char> colorDataTemp;
    //colorDataTemp = new QVector<char>;
    std::transform(deviceData.cbegin(), deviceData.cend(), std::back_inserter(colorDataTemp),
                   [this](double f) { return transformToColor(f); });
    return colorDataTemp;
}

char GLWidget::transformToColor(const double& data)
{
    if(data < 0.75f )
        return 0;
    else if(data < 0.8f )
        return 1;
    else if(data < 0.90f )
        return 2;
    else if(data < 0.93f )
        return 3;
    else if(data < 0.97f )
        return 4;
    else return 5;
}
