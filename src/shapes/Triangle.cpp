#include "Triangle.h"
#include "Shape.h"

void Triangle::updateParams() {
    m_vertexData = std::vector<float>();
    setVertexData();
}

void Triangle::setVertexData() {
    Shape::makeTriangle(glm::vec3(-0.5, 0.5, 0.0),
                        glm::vec3(-0.5, -0.5, 0.0),
                        glm::vec3(0.5, -0.5, 0.0),
                        m_vertexData);
}
