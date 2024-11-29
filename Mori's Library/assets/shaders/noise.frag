#version 450 core

//layout(location = 0) in vec3 vColor;

layout(location = 0) out vec4 fFragClr;

float random2d(vec2 coord){
    return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

uniform vec4 uFillColor;
uniform float uTime;
uniform vec2 uResolution;
//uniform bool uDepart;

void main()
{
    vec2 coord = gl_FragCoord.xy / uResolution;

    vec4 color = uFillColor;

    float noise = random2d(vec2(sin(coord))*uTime);
    //float noise2 = random2d(vec2(sin(coord)));
    //if(uDepart){
        color += noise * uTime;
    //}
    // else{
    //     if(uTime < 1){
    //         color += noise + sin(uTime);    
    //     }      
    //     else{
    //         color = uFillColor;
    //     }
      
    // }
    
    fFragClr = vec4(color);
    
}

