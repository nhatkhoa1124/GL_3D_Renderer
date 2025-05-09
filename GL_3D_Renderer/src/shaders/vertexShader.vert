#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main(){
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0f));
    
    mat4 mvp = projection * view * model;

    gl_Position = mvp * vec4( aPos, 1.0f);
}