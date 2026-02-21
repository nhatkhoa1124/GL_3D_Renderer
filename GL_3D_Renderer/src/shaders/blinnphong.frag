#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct PhongIntensity {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 color;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

#define MAX_POINT_LIGHTS 1

uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform samplerCube pointShadowMap;
uniform DirLight dirLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform Material material;
uniform PhongIntensity phongIntensity;
uniform float farPlane;

// --- SHADOW CALCULATIONS ---

float calcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(projCoords.z > 1.0) {
        return 0.0;
    }
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 2.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

// FIXED: 3D Point Shadow PCF
float calcPointShadow(vec3 fragPos, vec3 lightPos, vec3 normal) {
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    
    float shadow = 0.0;
    float bias = 0.15; // Point shadows often need a slightly larger, fixed bias
    float diskRadius = 0.05;
    
    // Array of 3D offsets for sampling the cubemap (Standard for soft point shadows)
    vec3 sampleOffsetDirections[20] = vec3[](
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    for(int i = 0; i < 20; i++) {
        float closestDepth = texture(pointShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;   // Undo [0,1] mapping
        
        if(currentDepth - bias > closestDepth) {
            shadow += 1.0;
        }
    }
    shadow /= 20.0;
    
    return shadow;
}

// --- LIGHTING CALCULATIONS ---

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shininess, vec4 fragPosLightSpace, vec3 texDiffuse, vec3 texSpecular) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfway = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfway), 0.0), shininess);

    float shadow = calcShadow(fragPosLightSpace, normal, lightDir);
    vec3 ambient = phongIntensity.ambient * light.color * texDiffuse;
    vec3 diffuse = diff * phongIntensity.diffuse * light.color * texDiffuse;
    vec3 specular = spec * phongIntensity.specular * light.color * texSpecular;

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess, vec3 texDiffuse, vec3 texSpecular) {
     if (length(light.color) < 0.001) {
        return vec3(0.0);
    }
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfway = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfway), 0.0), shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Calculate point light shadow
    float shadow = calcPointShadow(fragPos, light.position, normal);

    vec3 ambient = phongIntensity.ambient * light.color * texDiffuse;
    vec3 diffuse = diff * phongIntensity.diffuse * light.color * texDiffuse;
    vec3 specular = spec * phongIntensity.specular * light.color * texSpecular;
    
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

void main() {
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    // 1. Calculate textures safely inside main()!
    vec3 texDiffuseValue = mix(texture(material.texture_diffuse1, fs_in.TexCoords).rgb, texture(material.texture_diffuse2, fs_in.TexCoords).rgb, 0.5);
    vec3 texSpecularValue = mix(texture(material.texture_specular1, fs_in.TexCoords).rgb, texture(material.texture_specular2, fs_in.TexCoords).rgb, 0.5);

    // 2. Pass textures to lighting functions
    vec3 result = calcDirLight(dirLight, norm, viewDir, 32.0, fs_in.FragPosLightSpace, texDiffuseValue, texSpecularValue);
    
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLight[i], norm, fs_in.FragPos, viewDir, 32.0, texDiffuseValue, texSpecularValue);
    }

    FragColor = vec4(result, 1.0);
}