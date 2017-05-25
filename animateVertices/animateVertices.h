#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

class AnimateVertices : public QObject, public BasicPlugin {
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "BasicPlugin")
#endif
    Q_INTERFACES(BasicPlugin)

public:

    void onPluginLoad();
    void preFrame();
    void postFrame();

private:
    QOpenGLShader* vs;
    QOpenGLShader* fs;
    QOpenGLShaderProgram* program;
    QTimer timer;
    QElapsedTimer elapsedTimer;
};
 
 #endif
