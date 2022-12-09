#include "Cone.h"
#include "Shape.h"

Cone::Cone(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = std::max(3, param2);
    setVertexData();
}

void Cone::makeTile(glm::vec3 top,
                    glm::vec3 left,
                    glm::vec3 right,
                    bool tip,
                    glm::vec3 tipNormal) {

    Shape::insertVec3(m_vertexData, top);
    if (tip) {
        Shape::insertVec3(m_vertexData, glm::normalize(tipNormal));
    } else {
        Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(top[0], (0.5 - top[1]) / 4, top[2])));
    }
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, left);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(left[0], (0.5 - left[1]) / 4, left[2])));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, right);
    Shape::insertVec3(m_vertexData, glm::normalize(glm::vec3(right[0], (0.5 - right[1]) / 4, right[2])));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));
}

void Cone::makeWedge(float currentTheta, float nextTheta) {
    float yOffset = 1.f / m_param1;
    float rOffset = m_radius / m_param1;
    float halfStep = (nextTheta - currentTheta) / 2.f;

    auto top = glm::vec3();
    auto left = glm::vec3();
    auto right = glm::vec3();

    for (int i = 0; i < m_param1; i++) {
        // cone part
        float topY = 0.5 - (i * yOffset);
        float botY = 0.5 - ((i + 1) * yOffset);

        top = Shape::getXYZ(i * rOffset, nextTheta, topY);
        left = Shape::getXYZ((i + 1) * rOffset, currentTheta, botY);
        right = Shape::getXYZ((i + 1) * rOffset, nextTheta, botY);

        if (i == 0) {
            auto tipNormal = Shape::getXYZ(1, currentTheta + halfStep, topY);
            makeTile(top, right, left, true, tipNormal);
        } else {
            makeTile(top, right, left, false, glm::vec3());
        }

        top = Shape::getXYZ(i * rOffset, currentTheta, topY);
        left = Shape::getXYZ((i + 1) * rOffset, currentTheta, botY);
        right = Shape::getXYZ(i * rOffset, nextTheta, topY);
        makeTile(top, right, left, false, glm::vec3());

        // bottom cap
        Shape::makeCap(i, rOffset, currentTheta, nextTheta, m_vertexData, false);
    }
}


void Cone::setVertexData() {
    float thetaOffset = glm::radians(360.f) / m_param2;
    for (int i = 0; i < m_param2; i++) {
        makeWedge(i * thetaOffset, (i + 1) * thetaOffset);
    }
}
