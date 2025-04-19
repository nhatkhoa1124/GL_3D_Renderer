#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

void main()
{    
    vec4 diffuseColor = texture(material.texture_diffuse1, TexCoord);
    vec4 specularColor = texture(material.texture_specular1, TexCoord); 

    // Just add them to make OpenGL happy (you can replace with real lighting later)
    FragColor = diffuseColor + specularColor;

}
