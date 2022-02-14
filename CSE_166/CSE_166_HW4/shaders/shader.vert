#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int mode;

out vec3 fragNormal;
out vec3 fragVert;

void main() {
    gl_Position = projection * view * model * vec4(position, 1);

    // Final stuff to pass to frag shader
    fragNormal = normal;
    fragVert = position;
}