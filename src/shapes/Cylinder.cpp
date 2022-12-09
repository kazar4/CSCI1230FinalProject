#include "Cylinder.h"
#include "Shape.h"

Cylinder::Cylinder(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = std::max(3, param2);
    setVertexData();
}

void Cylinder::makeTile(glm::vec3 topLeft,
                        glm::vec3 topRight,
                        glm::vec3 bottomLeft,
                        glm::vec3 bottomRight) {
    Shape::insertVec3(m_vertexData, topLeft);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(topLeft[0], 0, topLeft[2])));
    Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, bottomLeft);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomLeft[0], 0, bottomLeft[2])));
    Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, topRight);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(topRight[0], 0, topRight[2])));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, bottomLeft);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomLeft[0], 0, bottomLeft[2])));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, bottomRight);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0, bottomRight[2])));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, topRight);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(topRight[0], 0,  topRight[2])));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));
}

void Cylinder::makeWedge(float currentTheta, float nextTheta) {
    float yOffset = 1.f / m_param1;
    float rOffset = m_radius / m_param1;

    for (int i = 0; i < m_param1; i++) {
        // cylindrical portion
        float topY = 0.5 - (i * yOffset);
        float botY = 0.5 - ((i + 1) * yOffset);
        auto tl = Shape::getXYZ(0.5, currentTheta, topY);
        auto tr = Shape::getXYZ(0.5, nextTheta, topY);
        auto bl = Shape::getXYZ(0.5, currentTheta, botY);
        auto br = Shape::getXYZ(0.5, nextTheta, botY);
        makeTile(tr, tl, br, bl);

        // caps
        Shape::makeCap(i, rOffset, currentTheta, nextTheta, m_vertexData, true);
    }
}

void Cylinder::setVertexData() {
    float thetaOffset = glm::radians(360.f) / m_param2;
    for (int i = 0; i < m_param2; i++) {
        makeWedge(i * thetaOffset, (i + 1) * thetaOffset);
    }
}
