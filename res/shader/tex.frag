#version 330 core
in vec2 out_tex_coord;
in vec4 out_color;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = out_color * texture(tex, out_tex_coord);
}
