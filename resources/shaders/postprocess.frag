//#version 330 core

//in vec2 uvCoord;

//uniform sampler2D Texture0;
//uniform bool applyInvert;
//uniform bool applyBlur;
//uniform float kernel[25];
//uniform int width;
//uniform int height;

//out vec4 fragColor;

//void convolve2D() {
//    int kernelLength = 5;
//    int kernelRadius = (kernelLength - 1) / 2;

//    float u = uvCoord[0];
//    float v = uvCoord[1];

//    for (int i = 0; i < kernelLength; i++) {
//        for (int j = 0; j < kernelLength; j++) {
//            float newU = u - (float(kernelRadius) / float(width)) + (float(j) / float(width));
//            float newV = v - (float(kernelRadius) / float(height)) + (float(i) / float(height));
//            vec4 pixelColor = texture(Texture0, vec2(newU, newV));

//            float kernelValue = kernel[j * kernelLength + i];

//            fragColor += pixelColor * kernelValue;
//        }
//    }
//}

#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

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


float shadowCalculation(vec4 fragPosLightSpace) {
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xy).r;
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // check whether current frag pos is in shadow
//    float bias = 0.005;
//    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

//    if (projCoords.z > 1.0) return 0.0;
//    return shadow;

    // perform perspective divide
       vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
       // transform to [0,1] range
       projCoords = projCoords * 0.5 + 0.5;
       // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
       float closestDepth = texture(shadowMap, projCoords.xy).r;
       // get depth of current fragment from light's perspective
       float currentDepth = projCoords.z;
       // calculate bias (based on depth map resolution and slope)
//       vec3 normal = normalize(fs_in.Normal);
//       vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//       float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
       float bias = 0.005;
       // check whether current frag pos is in shadow
       // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
       // PCF
       float shadow = 0.0;
       vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
       for(int x = -1; x <= 1; ++x)
       {
           for(int y = -1; y <= 1; ++y)
           {
               float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
               shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
           }
       }
       shadow /= 9.0;

       // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
       if(projCoords.z > 1.0)
           shadow = 0.0;

       return shadow;
}

void main() {

    fragColor = vec4(1.0);

    fragColor = vec4(0, 0, 0, 1.0);

    vec3 worldPos = fs_in.FragPos;
    vec3 worldNorm = normalize(fs_in.Normal);


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

    float shadow = shadowCalculation(fs_in.FragPosLightSpace);
    fragColor = (k_a * cAmbient) + ((1.0 - shadow) * fragColor);

}
