#version 130

uniform float checkSize;
out vec4 fragColor;
in vec2 surfacePos;

void main(void)
{
      vec2 position = surfacePos / checkSize;
      if(
        (fract(position.x) < 0.5 && fract(position.y) < 0.5) ||
        (fract(position.x) > 0.5 && fract(position.y) > 0.5)
      ){
        fragColor = vec4(0,0,0,1.0);
      } else{
        fragColor = vec4(1,1,1,1.0);
      }
}
