#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 viewPos;

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

uniform DirLight dirLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform PhongIntensity phongIntensity;

vec3 texDiffuseValue = mix(texture(material.texture_diffuse1, TexCoord).rgb, texture(material.texture_diffuse2, TexCoord).rgb, 0.5f);
vec3 texSpecularValue = mix(texture(material.texture_specular1, TexCoord).rgb, texture(material.texture_specular2, TexCoord).rgb, 0.5f);

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shininess) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = vec3(0.05f) * light.color * texDiffuseValue;
	vec3 diffuse = diff * vec3(0.4) * light.color * texDiffuseValue;
	vec3 specular = spec * vec3(0.5) * light.color * texSpecularValue;
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = phongIntensity.ambient * light.color * texDiffuseValue;
	vec3 diffuse = diff * phongIntensity.diffuse * light.color * texDiffuseValue;
	vec3 specular = spec * phongIntensity.specular * light.color * texSpecularValue;
	return (ambient + diffuse + specular) * attenuation;
}

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcDirLight(dirLight, norm, viewDir, 32.0);
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		result += calcPointLight(pointLight[i], norm, FragPos, viewDir, 32.0);
	}

	FragColor = vec4(result, 1.0);
}