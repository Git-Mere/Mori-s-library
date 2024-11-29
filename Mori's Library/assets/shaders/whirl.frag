#version 450 core

layout(location = 0) out vec4 fFragClr;

uniform vec4 uFillColor;
uniform vec2 uResolution;
uniform float uTime;
uniform bool uDepart;

void main()
{
    vec2 coord = gl_FragCoord.xy / uResolution;

    vec3 color = vec3(1.0);

    float angle = atan(-coord.y + 0.25, coord.x - 0.5)* 0.1;

    float len = length(coord - vec2(0.5,0.25));

    //color += sin(len * 50.0 + angle * 40.0 + uTime * 3.);
    color.r += sin(len * 40.0 + angle * 50.0 + uTime);
    color.g += cos(len * 30.0 + angle * 60.0 - uTime);
    color.b += sin(len * 50.0 + angle * 50.0 + 3.0);
    float alpha = mix(0,1, uTime / 6);

    //color.a += sin(len * 40.0 + angle * 50.0 + u_time);

    fFragClr = vec4(color, alpha);
}

