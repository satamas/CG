#include "circlegeometry.h"
#include <cmath>

CircleGeometry::CircleGeometry(float radius, unsigned int sectors, float h):
    _radius(radius),
    _sectors(sectors),
    _h(h),
    _vboIds(NUMBER_OF_VBO)
{
}

void CircleGeometry::initGeometry(){
    float const S = 1./(float)(_sectors-1);
    int r, s;

    _vertices.reserve(_sectors + 1);
    _vertices.push_back({QVector3D(0,_h,0), QVector2D(0.5, 0.5)});
    for(s = 0; s < _sectors; s++) {
        float const y = _h;
        float const x = cos(2*M_PI * s * S);
        float const z = sin(2*M_PI * s * S);
        _vertices.push_back({QVector3D(x * _radius, y, z * _radius), QVector2D(0.5 + x * 0.5, 0.5 + z * 0.5)});
    }

    _indices.reserve(_sectors);
    _indices.push_back(0);
    for(s = 0; s < _sectors; s++) {
        _indices.push_back(s + 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexData), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), _indices.data(), GL_STATIC_DRAW);
}

void CircleGeometry::init() {
    initializeGLFunctions();
    glGenBuffers(NUMBER_OF_VBO, _vboIds.data());
    initGeometry();
}


void CircleGeometry::draw(QGLShaderProgram *program) {
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
    glDrawElements(GL_TRIANGLE_FAN, _indices.size(), GL_UNSIGNED_SHORT, 0);
}

