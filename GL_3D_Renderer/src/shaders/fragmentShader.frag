#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;

void main()
{    
    vec3 diffuse = (texture(material.texture_diffuse1, TexCoord) + texture(material.texture_diffuse2, TexCoord)).rgb;
    vec3 specular = (texture(material.texture_specular1, TexCoord) + texture(material.texture_specular2, TexCoord)).rgb;

    // Combine diffuse textures (average)
    vec3 combinedDiffuse = diffuse* 0.5;

    // Combine specular textures (average, with smaller influence)
    vec3 combinedSpecular = specular * 0.005;

    FragColor = vec4(combinedDiffuse + combinedSpecular,1.0);
}