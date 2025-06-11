#version 330 core
in vec3 pos;
in vec4 color;
in vec3 rot_params;

out vec4 out_color;

uniform mat4 proj;

void main() {
    out_color = color;

    float c = cos(rot_params.z);
    float s = sin(rot_params.z);
    float xtr = -rot_params.x * c + rot_params.y * s + rot_params.x;
    float ytr = -rot_params.x * s - rot_params.y * c + rot_params.y;

    mat4 rot = mat4(
        vec4( c,   s,   0.0, 0.0),
        vec4(-s,   c,   0.0, 0.0),
        vec4( 0.0, 0.0, 1.0, 0.0),
        vec4( xtr, ytr, 0.0, 1.0)
    );

    float z = -1.0 / pos.z;
    gl_Position = proj * rot * vec4(pos.xy, z, 1.0);
}
