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
	float intentsity;
	vec3 direction;
};

struct PointLight {
	vec3 color;
	float intentsity;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};
struct SpotLight {
	vec3 color;
	float intentsity;
	vec3 position;
	vec3 spotDirection;
	float innerCutOff;
	float outerCutOff;
};

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
};

#define MAX_POINT_LIGHTS 2

uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform DirLight dirLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform PhongIntensity phongIntensity;

vec3 texDiffuseValue = mix(texture(material.texture_diffuse1, fs_in.TexCoords).rgb, texture(material.texture_diffuse2, fs_in.TexCoords).rgb, 0.5);
vec3 texSpecularValue = mix(texture(material.texture_specular1, fs_in.TexCoords).rgb, texture(material.texture_specular2, fs_in.TexCoords).rgb, 0.5);

float calcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
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
	if(projCoords.z > 1.0) {
		shadow = 0.0;
	}
	return shadow;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shininess, vec4 fragPosLightSpace) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 halfway = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, halfway), 0.0), shininess);

	float shadow = calcShadow(fragPosLightSpace, normal, lightDir);
	vec3 ambient = vec3(0.15) * light.color * texDiffuseValue;
	vec3 diffuse = diff * light.color * texDiffuseValue;
	vec3 specular = spec * light.color * texSpecularValue;

	return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 halfway = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, halfway), 0.0), shininess);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = phongIntensity.ambient * light.color * texDiffuseValue;
	vec3 diffuse = diff * phongIntensity.diffuse * light.color * texDiffuseValue;
	vec3 specular = spec * phongIntensity.specular * light.color * texSpecularValue;
	return (ambient + diffuse + specular) * attenuation;
}

void main() {
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec3 result = calcDirLight(dirLight, norm, viewDir, 32.0, fs_in.FragPosLightSpace);
	// for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
	// 	result += calcPointLight(pointLight[i], norm, fs_in.FragPos, viewDir, 32.0);
	// }

	FragColor = vec4(result, 1.0);
}