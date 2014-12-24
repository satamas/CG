#include "spheregeometry.h"

SphereGeometry::SphereGeometry(float radius, unsigned int rings, unsigned int sectors) :
        _radius(radius),
        _rings(rings),
        _sectors(sectors),
        _vboIds(NUMBER_OF_VBO) {
    _vertices.reserve(_rings * _sectors * 3);
    _indices.reserve(_rings * _sectors * 4);
}

SphereGeometry::~SphereGeometry() {
    glDeleteBuffers(NUMBER_OF_VBO, _vboIds.data());
}

void SphereGeometry::init() {
    initializeGLFunctions();
    glGenBuffers(NUMBER_OF_VBO, _vboIds.data());
    initGeometry();
}

void SphereGeometry::initGeometry() {
    float const R = 1. / (float) (_rings - 1);
    float const S = 1. / (float) (_sectors - 1);
    int r, s;

    _vertices.clear();
    _indices.clear();

    for (r = 0; r < _rings; r++)
        for (s = 0; s < _sectors; s++) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
            _vertices.push_back({QVector3D(x * _radius, y * _radius, z * _radius), QVector2D((float) (_textureFactor * s * S), (float) (_textureFactor * r * R))});

//        *n++ = x;
//        *n++ = y;
//        *n++ = z;
        }

    for (r = 0; r < _rings - 1; r++)
        for (s = 0; s < _sectors - 1; s++) {
            _indices.push_back(r * _sectors + s);
            _indices.push_back(r * _sectors + (s + 1));
            _indices.push_back((r + 1) * _sectors + (s + 1));
            _indices.push_back((r + 1) * _sectors + s);
        }

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexData), _vertices.data(), GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), _indices.data(), GL_STATIC_DRAW);
}

//! [2]
void SphereGeometry::draw(QGLShaderProgram *program) {
    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);

    // Offset for position
    size_t offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *) offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *) offset);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_QUADS, _indices.size(), GL_UNSIGNED_SHORT, 0);
}
