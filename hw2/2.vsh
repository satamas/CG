#version 130

uniform mat4 mvp;
uniform vec3 color;
in vec3 vertex;
in vec3 norm;

out vec4 pass_norm;
out vec4 pass_color;
out vec4 pass_pos;

void main(void)
{
     gl_Position = mvp * vec4(vertex, 1.0);
     pass_norm = mvp * vec4(norm, 1.0);
     pass_color = vec4(color, 1.0);
     pass_pos = gl_Position;
}
