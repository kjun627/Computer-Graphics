#include "context.h"

ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if(!context->Init()){
        return nullptr;
    }
    return std::move(context);
}

bool Context::Init(){
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5, 0.5, 0.0f,
    };
    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    

    //vertex layout 을 활용
    m_vertexLayout = VertexLayout::Create();
    // buffer class 를 활용한 buffer gen -> binding 과정
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
    vertices, sizeof(float)*12);

    //vertex layout 을 활용
    m_vertexLayout->SetAttrib(0,3,GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

    // buffer class 를 활용한 buffer gen -> binding 과정
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
    indices, sizeof(float)* 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if(!vertShader || !fragShader){
        return false;
    }
    SPDLOG_INFO("vertex shader id {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
    m_program = Program::Create({fragShader, vertShader});
    if(!m_program){
        return false;
    }
    SPDLOG_INFO("program id: {}", m_program->Get());

    // fragment shader에서 설정한 uniform qulifier 의 handle 획득
    auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    // handle 을 얻은 직후에는 이런식으로 모든 vertex가 가지는 uniform value 를 control
    glUniform4f(loc, 1.0f, 1.0f, 0.0f,1.0f);
    glClearColor(0.0f, 0.1f, 0.2f,0.0f);

    return true;
}

void Context::Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    static float time = 0.0f;
    float t = sinf(time) * 0.5f + 0.5f; // sin (time) 값에 0.5를 곱하고 + 0.5를 하는 방식 -> 최대 1 , 최소값 0
    auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    //indexBuffer 를 사용하여 Draw 할 때는 다음 함수 사용
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    time += 0.016f;
}