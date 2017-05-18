#include "reflection.h"
#include <QCoreApplication>

const int IMAGE_WIDTH = 1024;
const int IMAGE_HEIGHT = IMAGE_WIDTH;

void Reflection::onPluginLoad() {
    GLWidget & g = *glwidget();
    g.makeCurrent();
    // Carregar shader, compile & link 
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/reflection.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/reflection.frag");

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();

    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/template.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/template.frag");

    mainProgram = new QOpenGLShaderProgram(this);
    mainProgram->addShader(vs);
    mainProgram->addShader(fs);
    mainProgram->link();

    // Setup texture
    g.glActiveTexture(GL_TEXTURE0);
    g.glGenTextures( 1, &textureId);
    g.glBindTexture(GL_TEXTURE_2D, textureId);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		      GL_LINEAR_MIPMAP_LINEAR );
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    g.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMAGE_WIDTH, IMAGE_HEIGHT,
		   0, GL_RGB, GL_FLOAT, NULL);
    g.glBindTexture(GL_TEXTURE_2D, 0);
    // Resize to power-of-two viewport
    g.resize(IMAGE_WIDTH,IMAGE_HEIGHT);
}

void drawRect(GLWidget &g) {
    static bool created = false;
    static GLuint VAO_rect;

    // 1. Create VBO Buffers
    if (not created) {
        created = true;
        

        // Create & bind empty VAO
        g.glGenVertexArrays(1, &VAO_rect);
        g.glBindVertexArray(VAO_rect);

        GLuint VBO_coords;
        g.glGenBuffers(1, &VBO_coords);
        g.glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
        g.glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        g.glEnableVertexAttribArray(0);
        g.glBindVertexArray(0);
    }

    // 2. Draw
    g.glBindVertexArray (VAO_rect);
    g.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    g.glBindVertexArray(0);
}

bool Reflection::paintGL() {
    GLWidget & g = *glwidget();
    // Pass 1. Draw scene
    g.glClearColor(1,1,1,0);
    g.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    QVector3D plane(0,1,0);
    int a = plane[0];
    int b = plane[1];
    int c = plane[2];
    QMatrix4x4 reflection(
                1-2*a*a, -2*a*b, -2*a*c, 0,
                -2*a*b,1-2*b*b, -2*b*c, 0,
                -2*a*c, -2*b*c, 1-2*c*c, 0,
                0, 0, 0, 1
                );
    mainProgram->bind();
    mainProgram->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix()*camera()->viewMatrix()*reflection);
    mainProgram->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    if (drawPlugin()) drawPlugin()->drawScene();

    // Get texture
    g.glBindTexture(GL_TEXTURE_2D, textureId);
    g.glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
			  IMAGE_WIDTH, IMAGE_HEIGHT);
    g.glGenerateMipmap(GL_TEXTURE_2D);

    // Pass 2. Draw quad using texture
    g.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    program->bind();
    program->setUniformValue("colorMap", 0);

    QMatrix4x4 transform;

    //transform.translate(0,-5,0);
    transform.rotate(90, 1,0,0);
    transform.scale(5);

// quad covering viewport 
    program->setUniformValue("modelViewProjectionMatrix",  camera()->projectionMatrix()*camera()->viewMatrix()*transform);

    drawRect(g);
    program->release();
    g.glBindTexture(GL_TEXTURE_2D, 0);

    mainProgram->bind();
    mainProgram->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix()*camera()->viewMatrix());
    mainProgram->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    if (drawPlugin()) drawPlugin()->drawScene();

    mainProgram->release();
    return true;
}
