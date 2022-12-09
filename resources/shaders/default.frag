#version 330 core

in vec3 worldPos;
in vec3 worldNorm;

out vec4 fragColor;

uniform float k_a;
uniform float k_d;
uniform float k_s;
uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform float shininess;

uniform vec4 lightPos[8];
uniform vec4 lightColor[8];
uniform vec4 lightDir[8];
uniform int lightType[8];
uniform vec3 lightFunc[8];
uniform float lightAngle[8];
uniform float lightPenumbra[8];
uniform int numLights;
uniform vec4 camPos;

float falloff(int i, float angle) {
    float angleInner = lightAngle[i] - lightPenumbra[i];
    if (angle <= angleInner) {
        return 1.f;
    } else if (angle > angleInner && angle <= lightAngle[i]) {
        float diff = (angle - angleInner) / (lightAngle[i] - angleInner);
        return 1 - (-2.f * pow(diff, 3) + 3.f * pow(diff, 2));
    } else {
        return 0.f;
    }
}

void main() {
    fragColor = vec4(1.0);

    fragColor = vec4(0, 0, 0, 1.0);
    fragColor += k_a * cAmbient;

    for (int i = 0; i < numLights; i++) {
        float d = length(vec3(lightPos[i]) - worldPos);
        float attenuation = 1 / (lightFunc[i][0] + (d * lightFunc[i][1]) + (pow(d, 2) * lightFunc[i][2]));
        vec3 L = normalize(vec3(lightPos[i]) - worldPos);

        // if directional
        if (lightType[i] == 0) {
            attenuation = 1.f;
            L = normalize(vec3(-lightDir[i]));
        }

        // diffuse
        vec4 diffuse = k_d * cDiffuse * clamp(dot(normalize(worldNorm), L), 0.0, 1.0);

        // specular
        vec3 R = normalize(reflect(-L, worldNorm));
        vec3 E = normalize(vec3(camPos) - worldPos);
        float specularComponent = (shininess == 0) ? 1.f : pow(clamp(dot(R, E), 0.0, 1.0), shininess);
        vec4 specular = k_s * cSpecular * specularComponent;


        vec4 lightIntensity =  lightColor[i];
        // if spotlight
        if (lightType[i] == 1) {
            vec3 lightDir = -vec3(lightDir[i]);
            float angle = acos(dot(L, lightDir) / (length(lightDir) * length(L)));
            lightIntensity *= falloff(i, angle);
        }

        fragColor += (attenuation * lightIntensity) * (diffuse + specular);
    }
}
