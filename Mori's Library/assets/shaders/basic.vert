#version 450 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 vTCoordinates;
layout(location = 2) in vec3 aVertexColor;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTextureCoordinates;

uniform mat3 uModelToNDC;
uniform float tex_coord_scale;
void main()
{
    vTextureCoordinates = vec2(vTCoordinates.x * tex_coord_scale, vTCoordinates.y );

    vec3 ndc    = uModelToNDC * vec3(aVertexPosition, 1.0);
    gl_Position = vec4(ndc.xy, 0.0, 1.0);

    vColor      = aVertexColor;
}
