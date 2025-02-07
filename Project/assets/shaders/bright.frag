#version 410 core

//layout(location = 0) in vec3 vColor;

layout(location = 0) out vec4 fFragClr;

uniform vec4 uFillColor;
uniform float uTime;

void main()
{
    fFragClr = vec4(uFillColor.rgb, uTime/2);
    
}

