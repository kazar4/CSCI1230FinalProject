#include "Shape.h"
#include <glm/glm.hpp>
#include <iostream>

float Shape::m_radius = 0.5;

// Inserts a glm::vec3 into a vector of floats.
void Shape::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Shape::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

void Shape::makeTriangle(glm::vec3 topLeft,
                         glm::vec3 botLeft,
                         glm::vec3 botRight,
                         std::vector<float>& m_vertexData) {

    auto a = glm::vec3();
    auto b = glm::vec3();

    Shape::insertVec3(m_vertexData, topLeft);
    a = botLeft - topLeft;
    b = botRight - topLeft;
    Shape::insertVec3(m_vertexData, glm::normalize(glm::cross(a, b)));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, botLeft);
    a = botRight - botLeft;
    b = topLeft - botLeft;
    Shape::insertVec3(m_vertexData, glm::normalize(glm::cross(a, b)));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));

    Shape::insertVec3(m_vertexData, botRight);
    a = topLeft - botRight;
    b = botLeft - botRight;
    Shape::insertVec3(m_vertexData, glm::normalize(glm::cross(a, b)));
        Shape::insertVec2(m_vertexData, glm::vec2(0, 0));
};

void Shape::makeSquare(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 botLeft,
                       glm::vec3 botRight,
                       std::vector<float>& m_vertexData) {
    makeTriangle(topLeft, botLeft, topRight, m_vertexData);
    makeTriangle(botLeft, botRight, topRight, m_vertexData);
}

void Shape::makeCap(int i, float rOffset, float currentTheta, float nextTheta, std::vector<float>& m_vertexData, bool makeTopCap) {
    // bottom cap
    auto topLeft = Shape::getXYZ(i * rOffset, currentTheta, -m_radius);
    auto topRight = Shape::getXYZ(i * rOffset, nextTheta, -m_radius);
    auto botLeft = Shape::getXYZ((i + 1) * rOffset, currentTheta, -m_radius);
    auto botRight = Shape::getXYZ((i + 1) * rOffset, nextTheta, -m_radius);
    Shape::makeSquare(topLeft, topRight, botLeft, botRight, m_vertexData);

    if (makeTopCap) {
        // top cap
        topLeft = Shape::getXYZ(i * rOffset, currentTheta, m_radius);
        topRight = Shape::getXYZ(i * rOffset, nextTheta, m_radius);
        botLeft = Shape::getXYZ((i + 1) * rOffset, currentTheta, m_radius);
        botRight = Shape::getXYZ((i + 1) * rOffset, nextTheta, m_radius);
        Shape::makeSquare(topRight, topLeft, botRight, botLeft, m_vertexData);
    }
}

// cylindrical coords (radius, theta, height)
glm::vec3 Shape::getXYZ(float r, float theta, float y) {
    return glm::vec3(r * glm::cos(theta), y, r * glm::sin(theta));
}
