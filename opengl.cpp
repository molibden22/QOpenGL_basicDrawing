#include "opengl.h"

// Shaders Source
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

OpenGL::OpenGL(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(500, 500);
    setAutoFillBackground(true);
}

void OpenGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    shaderProgram->link();

    m_posAttr = shaderProgram->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = shaderProgram->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = shaderProgram->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
}

void OpenGL::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 1, 1, 1);
      shaderProgram->setUniformValue(m_matrixUniform, matrix);

    static const GLfloat vertices[] = {
         0.0f,  0.366f,
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.366f,
    };

    static const GLfloat verticesQuad[] = {
        -0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
         0.0f,  1.0f,
        -1.0f,  0.0f,
         0.0f, -1.0f,
         1.0f,  0.0f,
    };

    static const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    };

    switch( chooseVal)
    {
    case 0:
        renderTriangle(vertices, colors);
        break;
    case 1:
        renderLines(vertices, colors);
        break;
    case 2:
        renderQuad(verticesQuad, colors);
        break;
    default:
        break;
    }

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
    shaderProgram->release();
    ++m_frame;
}

void OpenGL::renderTriangle(const GLfloat vertices[], const GLfloat colors[])
{
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGL::renderLines(const GLfloat vertices[], const GLfloat colors[])
{
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

   // glDrawArrays(GL_POINTS, 0, 6);
    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_LINES, 1, 2);
    glDrawArrays(GL_LINES, 2, 2);
}

void OpenGL::renderQuad(const GLfloat vertices[], const GLfloat colors[])
{
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_QUADS, 0, 4);
}

void OpenGL::animate()
{
    update();
}

void OpenGL::changeChooseVal()
{
    chooseVal += 1;
    if (chooseVal > 2)
        chooseVal = 0;
}

//void OpenGL::paintEvent(QPaintEvent *event)
//{
//    QPainter painter;
//    painter.begin(this);
//    painter.setRenderHint(QPainter::Antialiasing);
//    render();
//    painter.end();
//}

void OpenGL::paintGL()
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    render();
    painter.end();
}
