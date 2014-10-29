#version 130

uniform mat4 mvp;
in vec3 vertex;
out vec2 surfacePos;

void main(void)
{
     surfacePos = (vertex).xy;
     gl_Position = mvp * vec4(vertex, 1.0);
}
