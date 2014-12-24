#ifndef QUADGEOMETRY_H
#define QUADGEOMETRY_H

#include "geometry.h"

class QuadGeometry : public Geometry {
public:
    QuadGeometry(float length, float width);

    void init() override;

    void draw(QGLShaderProgram *program) override;

protected:
    void initGeometry() override;


private:
    float _length;
    float _width;
    std::vector<GLuint> _vboIds;
    std::vector<VertexData> _vertices;
    std::vector<GLushort> _indices;
    static int const NUMBER_OF_VBO = 2;
};

#endif // QUADGEOMETRY_H
