#include "quadgeometry.h"

QuadGeometry::QuadGeometry(float length, float width):
    _length(length),
    _width(width),
    _vboIds(NUMBER_OF_VBO)
{
}

void QuadGeometry::init()
{
    initializeGLFunctions();
    glGenBuffers(NUMBER_OF_VBO, _vboIds.data());
    initGeometry();
}


void QuadGeometry::initGeometry(){
    _vertices.push_back({QVector3D(-_length / 2,  -_width / 2, 0), QVector2D(0, 0)});
    _vertices.push_back({QVector3D(_length / 2, -_width / 2, 0), QVector2D(_textureFactor * _length, 0)});
    _vertices.push_back({QVector3D(_length / 2, _width / 2, 0), QVector2D(_textureFactor * _length, _textureFactor * _width)});
    _vertices.push_back({QVector3D(-_length / 2, _width / 2, 0), QVector2D(0, _textureFactor * _width)});

    _indices.push_back(0);
    _indices.push_back(1);
    _indices.push_back(2);
    _indices.push_back(3);

    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexData), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), _indices.data(), GL_STATIC_DRAW);
}

void QuadGeometry::draw(QGLShaderProgram *program)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);


    size_t offset = 0;
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    offset += sizeof(QVector3D);
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_QUADS, _indices.size(), GL_UNSIGNED_SHORT, 0);
}
