#pragma once

#include <vector>
#include <glm/glm.hpp>

class Cone
{
public:
    Cone(int param1, int param2);
    std::vector<float> generateShape() { return m_vertexData; }

private:
    void setVertexData();
    void makeWedge(float currentTheta, float nextTheta);
    void makeTile(glm::vec3 top,
                  glm::vec3 left,
                  glm::vec3 right,
                  bool tip,
                  glm::vec3 tipNormal);

    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
