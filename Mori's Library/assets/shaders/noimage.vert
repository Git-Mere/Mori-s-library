#version 410 core

layout(location = 0) in vec2 aVertexPosition;
//layout(location = 1) in vec3 aVertexColor;

//layout(location = 0) out vec3 vColor;
uniform mat3 uModelToNDC;

void main()
{
    vec3 ndc    = uModelToNDC * vec3(aVertexPosition, 1.0);
    gl_Position = vec4(ndc.xy, 0.0, 1.0);

    //vColor      = aVertexColor;
}
