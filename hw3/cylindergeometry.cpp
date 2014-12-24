#include "cylindergeometry.h"

CylinderGeometry::CylinderGeometry(float radius, float height, unsigned int rings, unsigned int sectors) :
        _radius(radius),
        _height(height),
        _rings(rings),
        _sectors(sectors),
        topCircle(radius, sectors, height / 2),
        bottomCircle(radius, sectors, -height / 2),
        _vboIds(NUMBER_OF_VBO),
        Geometry() {
}

void CylinderGeometry::init() {
    topCircle.init();
    bottomCircle.init();
    initializeGLFunctions();
    glGenBuffers(NUMBER_OF_VBO, _vboIds.data());
    initGeometry();
}

void CylinderGeometry::initGeometry() {
    float const R = 1. / (float) (_rings - 1);
    float const S = 1. / (float) (_sectors - 1);
    int r, s;

    _vertices.reserve(_rings * _sectors);
    for (r = 0; r < _rings; r++)
        for (s = 0; s < _sectors; s++) {
            float const y = r * R;
            float const x = cos(2 * M_PI * s * S);
            float const z = sin(2 * M_PI * s * S);
            _vertices.push_back({QVector3D(x * _radius, y * _height - _height / 2, z * _radius), QVector2D(s * S, r * R)});
        }

    _indices.reserve(_rings * _sectors * 4);
    for (r = 0; r < _rings - 1; r++)
        for (s = 0; s < _sectors - 1; s++) {
            _indices.push_back(r * _sectors + s);
            _indices.push_back(r * _sectors + (s + 1));
            _indices.push_back((r + 1) * _sectors + (s + 1));
            _indices.push_back((r + 1) * _sectors + s);
        }
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexData), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), _indices.data(), GL_STATIC_DRAW);
}

void CylinderGeometry::draw(QGLShaderProgram *program) {
    topCircle.draw(program);
    bottomCircle.draw(program);
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);


    size_t offset = 0;
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *) offset);

    offset += sizeof(QVector3D);
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *) offset);

    // Draw cube geometry using indices from VBO 1
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_QUADS, _indices.size(), GL_UNSIGNED_SHORT, 0);
}
