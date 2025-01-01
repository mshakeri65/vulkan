#version 450

layout(binding = 0) uniform UniformBufferObject {
    float scaleX;
    float scaleY;
    float offsetX;
    float offsetY;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(
        inPosition.x * ubo.scaleX + ubo.offsetX,
        inPosition.y * ubo.scaleY + ubo.offsetY,
        0.0,
        1.0
    );

    fragTexCoord = inTexCoord;
}
