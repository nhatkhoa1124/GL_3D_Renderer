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
    vec4 diffuse = mix(texture(material.texture_diffuse1, TexCoord),texture(material.texture_diffuse2, TexCoord), 0.5f);
    vec4 specular = mix(texture(material.texture_specular1, TexCoord),texture(material.texture_specular2, TexCoord), 0.5f);

    vec4 textureColor = diffuse * vec4(0.5, 0.5, 0.5, 1.0) + specular * vec4(0.5, 0.5, 0.5, 1.0);

    if(textureColor.a < 0.1) {
        discard; // Discard the fragment if alpha is less than 0.5
    }
    FragColor = textureColor;
}