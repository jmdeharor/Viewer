#ifndef _GLOWING_H
#define _GLOWING_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


class Reflection : public QObject, public BasicPlugin
{
     Q_OBJECT
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
     Q_INTERFACES(BasicPlugin)

 public:
    void onPluginLoad();
    bool paintGL();
 
 private:
    // Create VBO with (x,y,z) coordinates
    float coords[] = { -1, -1, 0,
                        1, -1, 0,
                       -1,  1, 0,
                        1,  1, 0};

    QOpenGLShaderProgram* program;
    QOpenGLShaderProgram* mainProgram;
    QOpenGLShader* vs;
    QOpenGLShader* fs;  
    GLuint textureId;
 };
 
 #endif
