#include "draw-vbo-ext.h"
#include "glwidget.h"

void DrawSmooth::onPluginLoad() {

}

void DrawSmooth::onObjectAdd() {
    Object& object = scene()->objects()[scene()->objects().size()-1];
    vector<GLuint> indices(3*object.faces().size());
    vector<float> vertices(3*object.vertices().size());
    vector<float> normals(3*object.vertices().size());
    vector<float> colors(3*object.vertices().size());
    vector<list<GLuint>> VF(object.vertices().size());
    int i = 0;
    for (const Face& face : object.faces()) {
        for (int j = 0; j < face.numVertices(); ++i, ++j) {
            int index = face.vertexIndex(j);
            indices[i] = index;
            VF[index].push_back(j);
        }
    }
    i = 0;
    for (Vertex& vertex : object.vertices()) {
        Point p = vertex.coord();
        vertices[i] = p.x();
        vertices[i+1] = p.y();
        vertices[i+2] = p.z();
        i += 3;
    }
    for (unsigned int v = 0; v < VF.size(); ++v) {
        list<GLuint>& faces = VF[v];
        QVector3D average;
        for (GLuint faceIndex : faces) {
            average += object.faces()[faceIndex].normal();
        }
        average /= faces.size();
        normals[3*v] = average.x();
        normals[3*v+1] = average.y();
        normals[3*v+2] = average.z();
    }
    GLuint VAO, vertexVBO, normalVBO, indexBufferID;

    GLWidget &g = *glwidget();

    g.makeCurrent();

    g.glGenVertexArrays(1, &VAO);
    g.glGenBuffers(1, &vertexVBO);
    g.glGenBuffers(1, &normalVBO);
    g.glGenBuffers(1, &indexBufferID);

    g.glBindVertexArray(VAO);

    g.glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(0);

    g.glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(1);

    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    g.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

    g.glBindVertexArray(0);
    g.glBindBuffer(GL_ARRAY_BUFFER,0);
    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    VAOs.push_back(VAO);
    numIndices.push_back(indices.size());
}

void DrawSmooth::onSceneClear() {

}

bool DrawSmooth::drawObject(int i) {
    GLWidget &g = *glwidget();
    g.glBindVertexArray(VAOs[i]);
    g.glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, 0);

    g.glBindVertexArray(0);
    return true;
}

bool DrawSmooth::drawScene() {
    GLWidget &g = *glwidget();
    g.makeCurrent();

    for(unsigned int i=0; i < VAOs.size(); ++i) {

        g.glBindVertexArray(VAOs[i]);
        g.glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, 0);

    }
    g.glBindVertexArray(0);
    return true;
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(draw-mooth, DrawSmooth)   // plugin name, plugin class
#endif



