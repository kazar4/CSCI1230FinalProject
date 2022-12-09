#include "sceneparser.h"
#include "glm/ext/matrix_transform.hpp"
#include "scenefilereader.h"

#include <chrono>
#include <memory>
#include <iostream>

glm::mat4 transform(SceneTransformation* st, glm::mat4 ctm) {
    switch (st->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            return glm::translate(ctm, st->translate);
        case TransformationType::TRANSFORMATION_SCALE:
            return glm::scale(ctm, st->scale);
        case TransformationType::TRANSFORMATION_ROTATE:
            return glm::rotate(ctm, st->angle, st->rotate);
        case TransformationType::TRANSFORMATION_MATRIX:
        return st->matrix * ctm;
    }
    return glm::mat4();
}

void dfs(std::vector<RenderShapeData> &shapes, SceneNode* root, glm::mat4 ctm) {
    for (SceneTransformation* transformation : root->transformations) {
        ctm = transform(transformation, ctm);
    }

    for (ScenePrimitive* prim : root->primitives) {
        RenderShapeData sd = RenderShapeData{*prim, ctm, glm::inverse(ctm), glm::inverse(glm::transpose(glm::mat3(ctm)))};
        shapes.push_back(sd);
    }

    for (SceneNode* child : root->children) {
        dfs(shapes, child, ctm);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    // populate renderData with global data, lights, and camera data
    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    renderData.lights = fileReader.getLights();

    auto root = fileReader.getRootNode();

    renderData.shapes.clear();

    glm::mat4 ctm(1); // identity matrix

    // populate renderData's list of primitives and their transforms.
    dfs(renderData.shapes, root, ctm);

    return true;
}
