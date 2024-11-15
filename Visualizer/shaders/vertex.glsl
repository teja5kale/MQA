#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelView;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragPosition;

void main() {
    fragPosition = vec3(modelView * vec4(position, 1.0));
    fragNormal = mat3(transpose(inverse(modelView))) * normal;
    gl_Position = projection * vec4(fragPosition, 1.0);
}