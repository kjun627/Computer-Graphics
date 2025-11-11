#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 modelTransform;
/* uniform mat4 invTransModelTransform  원래는 이런식으로 shader 밖에서 한번만 계산하면 됨
몬든 vertex에 동일한 Matrix 가 연산됨. 학습을 위해서 아래와 같이 함.*/

out vec3 normal;
out vec2 texCoord;
out vec3 position;
void main() {
    // gl Position 은 canonical -> clipsapce에서의 좌표임
    gl_Position = transform * vec4(aPos, 1.0);
    // normal에 model transform 을 적용하는 과정
    // normal을 유지하기 위해서는 model transformmatrix에
    // 역행렬을 normal에 적용해줘야함.
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    // 여기서의 Position 은 cam2world 이후 world coordi 에서의 좌표값
    position = (modelTransform * vec4(aPos,1.0f)).xyz;
}
