#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

//uniform sampler2D texture1;
struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emission;
    float shininess;
};

uniform Material material;

void main()
{             
    vec4 texColor = texture(material.texture_diffuse1, TexCoords);
    FragColor = texColor;
}