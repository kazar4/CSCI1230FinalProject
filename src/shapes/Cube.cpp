#include "Cube.h"
#include "Shape.h"

Cube::Cube(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {

    glm::vec3 xOffset = (topRight - topLeft) / float(m_param1);
    glm::vec3 yOffset = (bottomLeft - topLeft) / float(m_param1);

    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            float r = float(i);
            float c = float(j);
             auto tl = topLeft + (xOffset * r) + (yOffset * c);
             auto tr = topLeft + (xOffset * (r + 1)) + (yOffset * c);
             auto bl = topLeft + (xOffset * r) + (yOffset * (c + 1));
             auto br = topLeft + (xOffset * (r + 1)) + (yOffset * (c + 1));
             Shape::makeSquare(tl, tr, bl, br, m_vertexData);
        }
    }
}

void Cube::setVertexData() {
    // front
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    // back
    makeFace(glm::vec3(-0.5f,  -0.5f, -0.5f),
             glm::vec3( 0.5f,  -0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3( 0.5f, 0.5f, -0.5f));

    // right side
    makeFace(glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(0.5f, 0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f));

    // left side
    makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(-0.5f, 0.5f, -0.5f));

    // top
    makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3(0.5f, 0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(0.5f, 0.5f, 0.5f));

    // bottom
    makeFace(glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f));
}

