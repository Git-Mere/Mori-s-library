#version 410 core


layout(location = 0) out vec4 fFragClr;

uniform vec3 uFillColor;
uniform bool IsCircle;

void main()
{
    if(IsCircle){
        fFragClr = vec4(uFillColor, 0.5);
    }
    else{
        fFragClr = vec4(uFillColor, 1.0);
    }
    
}

