#version 330 core

layout(location = 0) in vec3 objPos;
layout(location = 1) in vec3 objNorm;

out vec3 worldPos;
out vec3 worldNorm;

uniform mat4 modelMat;
uniform mat3 modelMatForNorm;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main() {
    // using model and view matricies, transform the object space position and normal data into world space
    worldPos = vec3(modelMat * vec4(objPos, 1));
    worldNorm = normalize(modelMatForNorm * normalize(objNorm));

    // set gl_Position to the object space position transformed to clip space
    gl_Position = projectionMat * viewMat * modelMat * vec4(objPos, 1);
}
