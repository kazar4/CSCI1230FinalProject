#pragma once

#include <vector>
#include <glm/glm.hpp>

class Cube
{
public:
    Cube(int param1);
    std::vector<float> generateShape() { return m_vertexData; }

private:
    void setVertexData();
    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);

    std::vector<float> m_vertexData;
    int m_param1;
};
