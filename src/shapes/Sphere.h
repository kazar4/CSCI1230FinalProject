#pragma once

#include <vector>
#include <glm/glm.hpp>

class Sphere
{
public:
    Sphere(int param1, int param2);
    std::vector<float> generateShape() { return m_vertexData; }

private:
    void setVertexData();
    glm::vec3 sphericalToCartesian(float phi, float theta);
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeWedge(float currTheta, float nextTheta);

    std::vector<float> m_vertexData;
    float m_radius = 0.5;
    int m_param1;
    int m_param2;
};

