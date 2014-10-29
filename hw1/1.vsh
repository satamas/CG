#version 130

uniform mat4 mvp;
in vec3 vertex;
out vec3 pass_Color;

void main(void)
{
     gl_Position = mvp * vec4(vertex, 1.0);
     pass_Color = vec3(1, 1, 1);
}
