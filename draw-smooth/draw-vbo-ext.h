#ifndef _DRAW_VBO_EXT_H
#define _DRAW_VBO_EXT_H

#include <vector>
#include "basicplugin.h"

using namespace std;

class DrawSmooth : public QObject, public BasicPlugin {
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
     Q_INTERFACES(BasicPlugin)

 public:
 
    void onPluginLoad();
    void onObjectAdd();
    void onSceneClear();
    bool drawObject(int i);
    bool drawScene();
   
 private:
     vector<GLuint> VAOs;
     vector<unsigned int> numIndices;
 };
 
 #endif
 
 
