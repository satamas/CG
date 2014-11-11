#version 130

in vec4 pass_pos;
in vec4 pass_norm;
in vec4 pass_color;
out vec4 out_Color;

uniform vec3 center;
uniform float radius;
uniform float time_from_start;
uniform float frequency;
uniform float wave_vector_length;

void main(void)
{
      float r = abs(sin(frequency * time_from_start + wave_vector_length * length(pass_pos - vec4(center,1)) ));
      float g = abs(sin(1.1*frequency * time_from_start + wave_vector_length * length(pass_pos - vec4(center,1)) ));
      float b = abs(sin(0.9*frequency * time_from_start + wave_vector_length * length(pass_pos - vec4(center,1)) ));
      out_Color = vec4(r, g, b, 1);
}
