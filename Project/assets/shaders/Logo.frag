#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

// uniform float uTileSize;

// uniform bool uUseInterpolatedColor;
// uniform bool uUseProcedural_Color;
// uniform bool uUseTexture_Color;

uniform sampler2D uTex2d;

void main()
{
    //vec3 gridColor = vec3(1, 0, 1);
    //vec3 gridColor2 = vec3(0.0,0.68,0.94);
    
    //vec2 tileCoord = vec2(gl_FragCoord.xy) / uTileSize;

// vec4 interpolated_color = vec4(1);
//     if(uUseInterpolatedColor) 
      vec4  interpolated_color = vec4(vColor, 1.0);

vec4 procedural_color = vec4(1);
    // if(uUseProcedural_Color) {
    //     if (mod(floor(tileCoord.x) + floor(tileCoord.y), 2.0) == 0) {
    //         procedural_color = vec4(gridColor, 1.0);
    //     } else {
    //         procedural_color = vec4(gridColor2, 1.0);
    //     }
    // }

// vec4 texture_color = vec4(1);
//     if(uUseTexture_Color) 
     vec4   texture_color = texture(uTex2d, vTextureCoordinates);
     //vec4   texture_color = vec4(1);

    fFragClr = interpolated_color * procedural_color * texture_color;
    //fFragClr = vec4(vColor, 1.0);
}