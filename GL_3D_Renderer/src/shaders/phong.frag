#version 330 core

out vec4 FragColor; 

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D tex;
uniform vec3 viewPos;

struct Light{
	vec3 lightPos;
	vec3 lightColor;
};

struct Material{
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float shininess;
};

uniform Light light;
uniform Material material;


void main(){
	vec3 ambient = material.ambientStrength * light.lightColor;

	vec3 lightDir = normalize(light.lightPos - FragPos);
	float lambert = max(dot(normalize(Normal), lightDir),0.0f);
	vec3 diffuse = material.diffuseStrength * light.lightColor * lambert;

	vec3 reflectDir = reflect(-lightDir, normalize(Normal));
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(0.0f, dot(reflectDir, viewDir)), material.shininess);
	vec3 specular = material.specularStrength * light.lightColor * spec ;

	vec3 texColor = texture(tex, TexCoord).rgb;
	vec3 result = (ambient + diffuse + specular) * texColor;

	FragColor = vec4 (result, 1.0f);
}