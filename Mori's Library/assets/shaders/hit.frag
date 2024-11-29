#version 330 core

layout(location = 0) out vec4 fFragClr;

uniform vec4 uFillColor;
uniform float uTime;
uniform vec2 uResolution;
uniform vec4 uHelper;

void main()
{
    vec2 coord = gl_FragCoord.xy / uResolution;
    vec4 color = vec4(0.0);
    vec2 translate = vec2(-0.5);
    coord += translate;

    color.r += abs(0.1 + length(coord/2) - (sin( 0.17))) * 30;
    color.a += mix(abs(0.2 + length(coord/2) - (sin( 0.17))), 0, uTime / 2) ;

    fFragClr = color;
}



