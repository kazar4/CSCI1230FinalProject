#include "Sphere.h"
#include "Shape.h"

Sphere::Sphere(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(2, param1); // phi
    m_param2 = std::max(3, param2); // theta
    setVertexData();
}

glm::vec3 Sphere::sphericalToCartesian(float phi, float theta) {
    return glm::vec3(m_radius * glm::sin(theta) * glm::sin(phi),
                     m_radius * glm::cos(phi),
                     m_radius * glm::cos(theta) * glm::sin(phi));
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    Shape::insertVec3(m_vertexData, topLeft);
    Shape::insertVec3(m_vertexData, glm::normalize(topLeft));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, bottomLeft);
    Shape::insertVec3(m_vertexData, glm::normalize(bottomLeft));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, bottomRight);
    Shape::insertVec3(m_vertexData, glm::normalize(bottomRight));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, topLeft);
    Shape::insertVec3(m_vertexData, glm::normalize(topLeft));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, bottomRight);
    Shape::insertVec3(m_vertexData, glm::normalize(bottomRight));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, topRight);
    Shape::insertVec3(m_vertexData, glm::normalize(topRight));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float phiOffset = glm::radians(180.f) / m_param1;

    for (int i = 0; i < m_param1; i++) {
        auto tl = sphericalToCartesian(i * phiOffset, currentTheta);
        auto tr = sphericalToCartesian(i * phiOffset, nextTheta);
        auto bl = sphericalToCartesian((i + 1) * phiOffset, currentTheta);
        auto br = sphericalToCartesian((i + 1) * phiOffset, nextTheta);
        makeTile(tl, tr, bl, br);
    }
}

void Sphere::setVertexData() {
    float thetaOffset = glm::radians(360.f) / m_param2;
    for (int i = 0; i < m_param2; i++) {
        makeWedge(i * thetaOffset, (i + 1) * thetaOffset);
    }
}
