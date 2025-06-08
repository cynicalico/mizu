#version 330 core
in vec3 pos;
in vec3 color;

out vec3 out_color;

uniform mat4 proj;

void main() {
    out_color = color;

    float z = -1.0 / pos.z;
    gl_Position = proj * vec4(pos.x + 0.5, pos.y + 0.5, z, 1.0);
}
