#include "modelInfo.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

void ModelInfo::onPluginLoad() {
    numFaces = numVertices = numTriangles = 0;
}

void ModelInfo::onObjectAdd() {
    unsigned int numObjects = scene()->objects().size();
    const vector<Face>& faces = scene()->objects()[numObjects-1].faces();
    numFaces += faces.size();
    unsigned int lastNumVertices = scene()->objects()[numObjects-1].vertices().size();
    numVertices += lastNumVertices;
    for (unsigned int i = 0; i < faces.size(); ++i) {
        if (faces[i].numVertices() == 3) ++numTriangles;
    }
    cout << "Total objects: " << numObjects << ", total faces: " << numFaces << ", total vertices " << numVertices << endl;
    cout << "% of triangles: " << (float(numTriangles)/numFaces)*100 << endl;
}

void ModelInfo::onSceneClear() {
    numFaces = numVertices = numTriangles = 0;
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(modelInfo1, ModelInfo)   // plugin name, plugin class
#endif

