#pragma once

#include <vector>
#include <glm/glm.hpp>

class Cylinder
{
public:
    Cylinder(int param1, int param2);
    std::vector<float> generateShape() { return m_vertexData; }

private:
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeWedge(float currTheta, float nextTheta);

    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
