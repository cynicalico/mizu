#version 330 core
in vec3 pos;
in vec3 color;

out vec3 out_color;

uniform mat4 proj;

void main() {
    out_color = color;

    gl_Position = proj * vec4(pos, 1.0);
}
