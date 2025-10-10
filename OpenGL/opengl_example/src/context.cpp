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

    // 버퍼 오브젝트 만들기 전에 VAO를 만들기
    // 1개의 오브젝트를 만들거고 그 아이디를 m_vertexArrayObject 라는 애한테 아이디 할당
    glGenVertexArrays(1, &m_vertexArrayObject);
    // 사용할 VAO binding
    glBindVertexArray(m_vertexArrayObject);
    
    // 버퍼 1개를 생성하고 생성된 아이디를 m_vertexBuffer에 할당
    glGenBuffers(1, &m_vertexBuffer);
    // Array_Buffer 에 m_vertexBuffer 가 가진 아이디를 바인딩 -> GL_Array_Buffer 라는 이름으로 VBO를 사용가능
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    // Array_Buffer에 float형 데이터 vertex 18개(사각형)를 삽입하겠다. 그리고 그 정보는 vertice의 데이터이다.
    // GL_STATIC_DRAW -> 한번 설정하고 여러번 사용
    // ( STATIC, DYNAMIC, STREAM ) * (DRAW, COPY , READ) 의 조합으로 사용 가능 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 , vertices,GL_STATIC_DRAW);

    // 특정 Vertex Attribute를 활성화
    // vertex shader의 location 
    //layout (location = 0) in vec3 aPos;
    // vertex shader의 input attribute 를 사용하게 하는
    glEnableVertexAttribArray(0);
    // attribute 위치(위에랑 동일) , 컴포넌트 개수(한 vertex당 몇개의 값을 가지는지 vector의 차원(?)), 
    // 데이터 타입, 정규화 여부 , 스트라이드, 오프셋 순서의 파라미터
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(float)* 3, 0);

    // 하나의 버퍼아이디를 부여 받아서
    glGenBuffers(1, &m_indexBuffer);
    // 부여받은 버퍼아이디를 인덱스 버퍼로 바인딩하고
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    // 인덱스 버퍼로  indices 에 담겨있었던 데이터들을 복사한다.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)* 6, indices, GL_STATIC_DRAW);

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
    glClearColor(0.0f, 0.1f, 0.2f,0.0f);

    return true;
}

void Context::Render(){
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program->Get());
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    //indexBuffer 를 사용하여 Draw 할 때는 다음 함수 사용
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}