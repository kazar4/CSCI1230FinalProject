#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 lightSpaceMatrix;


void main()
{
    fs_in.FragPos = vec3(modelMat * vec4(aPos, 1.0));
    fs_in.Normal = transpose(inverse(mat3(modelMat))) * aNormal;
    fs_in.TexCoords = aTexCoords;
    fs_in.FragPosLightSpace = lightSpaceMatrix * vec4(fs_in.FragPos, 1.0);
    gl_Position = projectionMat * viewMat * vec4(fs_in.FragPos, 1.0);
}
