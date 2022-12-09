#pragma once

#include "camera/camera.h"
#include "utils/scenedata.h"
#include "utils/sceneparser.h"

/**
 * @brief The RayTraceScene class representing a scene to be ray-traced
 */
class RayTraceScene
{
public:
    int w;
    int h;
    RenderData m_data;

    RayTraceScene();
    RayTraceScene(int width, int height, const RenderData &metaData);

    // The getter of the width of the scene
    const int& getWidth() const;

    // The getter of the height of the scene
    const int& getHeight() const;

    // The getter of the global data of the scene
    const SceneGlobalData& getGlobalData() const;

    // The getter of the shape data of the scene
    const std::vector<RenderShapeData>& getShapes() const;

    // The getter of the light data of the scene
    const std::vector<SceneLightData>& getLights() const;
};
