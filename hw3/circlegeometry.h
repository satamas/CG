#ifndef CIRCLEGEOMETRY_H
#define CIRCLEGEOMETRY_H

#include "geometry.h"

class CircleGeometry : public Geometry
{
public:
    CircleGeometry(float radius, unsigned int sectors, float h);
    void init() override;

    void draw(QGLShaderProgram *program) override;

protected:
    void initGeometry() override;
private:
    float _radius;
    unsigned int _sectors;
    float _h;

    std::vector<GLuint> _vboIds;
    std::vector<VertexData> _vertices;
    std::vector<GLushort> _indices;
    static int const NUMBER_OF_VBO = 2;
};

#endif // CIRCLEGEOMETRY_H
