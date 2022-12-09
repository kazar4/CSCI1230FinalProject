#include <iostream>
#include <stdexcept>
#include "camera.h"
#include "settings.h"

Camera::Camera() {};

/**
 * @brief Camera::Camera constructor for a Camera object
 * @param camData the SceneCameraData with information about the camera
 * @param width the width of the output
 * @param height the height of the output
 */
Camera::Camera(SceneCameraData camData, int width, int height) {
    position = camData.pos;
    look = camData.look;
    up = camData.up;
    heightAngle = camData.heightAngle;
    w = width;
    h = height;
}

/**
 * @brief Camera::getViewMatrix
 * @return the view matrix for the camera
 */
glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 translate = glm::mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -position[0], -position[1], -position[2], 1);


    glm::vec3 w = -look / glm::length(look);

    auto upCalc = glm::vec3(up) - glm::dot(glm::vec3(up), w) * w;
    glm::vec v = upCalc / glm::length(upCalc);

    auto u = glm::cross(v, w);

    glm::mat4 rotate = glm::mat4(u[0], v[0], w[0], 0,
                                u[1], v[1], w[1], 0,
                                u[2], v[2], w[2], 0,
                                0, 0, 0, 1);

    return rotate * translate;
}


/**
 * @brief Camera::getProjectionMatrix
 * @return the projection matrix for the camera
 */
glm::mat4 Camera::getProjectionMatrix() const {
    auto widthAngle = heightAngle * getAspectRatio();
    glm::mat4 scale = glm::mat4(1.f / (settings.farPlane * tan(widthAngle / 2.f)), 0, 0, 0,
                                0, 1.f / (settings.farPlane * tan(heightAngle / 2.f)), 0, 0,
                                0, 0, 1.f / settings.farPlane, 0,
                                0, 0, 0, 1.f);

    float c = -settings.nearPlane / settings.farPlane;
    glm::mat4 unhinge = glm::mat4(1.f, 0, 0, 0,
                                  0, 1.f, 0, 0,
                                  0, 0, 1.f / (1.f + c), -1.f,
                                  0, 0, -c / (1.f + c), 0);

    glm::mat4 remapZ = glm::mat4(1.f, 0, 0, 0,
                                 0, 1.f, 0, 0,
                                 0, 0, -2.f, 0,
                                 0, 0, -1.f, 1.f);

    return remapZ * unhinge * scale;
}

/**
 * @brief Camera::getAspectRatio
 * @return the aspect ratio of output image
 */
float Camera::getAspectRatio() const {
    return (float) w / h;
}


void Camera::updatePos(glm::vec4 newPos) {
    position = newPos;
}

void Camera::updateLook(glm::vec4 newLook) {
    look = newLook;
}

