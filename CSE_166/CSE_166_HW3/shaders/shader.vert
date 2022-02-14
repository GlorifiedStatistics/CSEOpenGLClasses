#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int mode;

uniform int DRAW_NORMALS;
uniform int DRAW_COLOR;
uniform int DRAW_REFLECTION;
uniform int DRAW_CUBEMAP;
uniform int DRAW_PHONG;
uniform int DRAW_FLOOR;

out vec3 fragNormal;
out vec3 fragVert;
out vec3 texCoords;

void main() {
    texCoords = position;
    
    if (mode == DRAW_CUBEMAP) {
        vec4 pos = projection * view * vec4(position, 1);
        gl_Position = pos.xyww;
    }
    else {
        gl_Position = projection * view * model * vec4(position, 1);
    }

    // Final stuff to pass to frag shader
    fragNormal = normal;
    fragVert = position;
}