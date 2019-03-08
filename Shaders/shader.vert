#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm; 

out vec4 vertColor;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    TexCoord = tex;

    Normal = mat3(transpose(inverse(model))) * norm;
}