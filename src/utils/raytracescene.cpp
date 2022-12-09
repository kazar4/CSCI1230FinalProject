#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include <iostream>

RayTraceScene::RayTraceScene() {};

RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData) {
    w = width;
    h = height;
    m_data = metaData;
}

const int& RayTraceScene::getWidth() const {
    return w;
}

const int& RayTraceScene::getHeight() const {
    return h;
}

const SceneGlobalData& RayTraceScene::getGlobalData() const {
    return m_data.globalData;
}

const std::vector<RenderShapeData>& RayTraceScene::getShapes() const {
    return m_data.shapes;
}

const std::vector<SceneLightData>& RayTraceScene::getLights() const {
    return m_data.lights;
}
