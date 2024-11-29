#version 450 core

layout(location = 0) out vec4 fFragClr;

uniform vec2 uResolution;
uniform float uTime;
uniform bool uDepart;

void main()
{
    vec2 coord = gl_FragCoord.xy / uResolution;
    // vec2 translate = vec2(-0.5);
    // coord += translate;
    vec3 color = vec3(1.0);

    float alpha = 0;
    if(uDepart){
         alpha = mix(0.0, 1.0, uTime / 3.);
         float gradient = dot(coord, normalize(vec2(1.0, 1.0)));
        gradient = clamp(gradient, 0.0, 1.0);
        color *= gradient;
        fFragClr = vec4(color, alpha);
    }
    else{
         alpha = mix(0.0, 1.0, uTime / 3.); 
         float gradient = dot(coord, normalize(vec2(-1.0, -1.0)));
        gradient = clamp(gradient, 0.0, 1.0);
        color *= gradient;
        fFragClr = vec4(color, alpha);
    }
    
}