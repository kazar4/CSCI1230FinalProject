#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>

// A class representing a virtual camera.
class Camera {
public:
    // constructors
    Camera();
    Camera(SceneCameraData camData, int width, int height);

    glm::vec4 position;
    glm::vec4 look;
    glm::vec4 up;
    float heightAngle;
    int w;
    int h;

    // Returns the view matrix for the current camera settings.
    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Setter for camera's position vector
    void updatePos(glm::vec4 newPos);

    // Setter for camera's look vector
    void updateLook(glm::vec4 newLook);
};
