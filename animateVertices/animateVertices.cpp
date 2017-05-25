#include "animateVertices.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

void AnimateVertices::onPluginLoad() {
    // Carregar shader, compile & link
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/animateVertices/animate-vertices1.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;

   fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/animateVertices/animate-vertices1.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;

    connect(&timer, SIGNAL(timeout()), glwidget(), SLOT(update()));
    timer.start();
    elapsedTimer.start();
}

void AnimateVertices::preFrame() {
    // bind shader and define uniforms
    program->bind();
    program->setUniformValue("time", float(elapsedTimer.elapsed()/1000.));
}

void AnimateVertices::postFrame() {
    program->release();
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(AnimateVertices, AnimateVertices)   // plugin name, plugin class
#endif

