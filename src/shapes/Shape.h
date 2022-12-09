#pragma once

#include <vector>
#include <glm/glm.hpp>

class Shape
{
public:
    static float m_radius;

    static void insertVec3(std::vector<float> &data, glm::vec3 v);

    static void insertVec2(std::vector<float> &data, glm::vec2 v);

    static void makeTriangle(glm::vec3 topLeft,
                             glm::vec3 bottomLeft,
                             glm::vec3 topRight,
                             std::vector<float>& m_vertexData);

    static void makeSquare(glm::vec3 topLeft,
                           glm::vec3 topRight,
                           glm::vec3 botLeft,
                           glm::vec3 botRight,
                           std::vector<float>& m_vertexData);

    static void makeCap(int i,
                        float rOffset,
                        float currentTheta,
                        float nextTheta,
                        std::vector<float>& m_vertexData,
                        bool makeTopCap);

    // cylindrical (radius, theta, y height)
    static glm::vec3 getXYZ(float r, float theta, float y);
};

