#version 130

in vec4 pass_norm;
in vec4 pass_color;
out vec4 out_Color;

uniform bool color_from_norm;

void main(void)
{
    if(color_from_norm){
        out_Color = vec4(pass_norm.y, pass_norm.y, pass_norm.y, 1);
    } else{
        out_Color = pass_color;
    }
}
