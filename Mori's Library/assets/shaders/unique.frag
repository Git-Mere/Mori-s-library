#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;

void main()
{
    vec4 interpolated_color = vec4(1); 
    vec4 procedural_color = vec4(1);
    vec4 texture_color = texture(uTex2d, vTextureCoordinates);

    if (texture_color.a == 0.0) {
        fFragClr = vec4(0, 0, 0, 0.5);
    } else {
        fFragClr = interpolated_color * procedural_color * texture_color;
    }
}
