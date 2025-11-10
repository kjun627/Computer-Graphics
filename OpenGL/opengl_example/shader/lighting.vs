#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 modelTransform;

out vec3 normal;
out vec2 texCoord;
out vec3 position;
void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    // normal에 model transform 을 적용하는 과정
    // normal을 유지하기 위해서는 model transformmatrix에
    // 역행렬을 normal에 적용해줘야함.
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    position = (modelTransform * vec4(aPos,1.0f)).xyz;
}
