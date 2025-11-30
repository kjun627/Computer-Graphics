#include "context.h"
#include "image.h"
#include <imgui.h>

ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if(!context->Init()){
        return nullptr;
    }
    return std::move(context);
}

void Context::ProcessInput(GLFWwindow* window){
    if(!m_cameraControl) return;
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp , -m_cameraFront));
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape (int width, int height){
    m_width = width;
    m_height = height;
    glViewport(0,0, m_width, m_height);
}

void Context::MouseMove(double x, double y){
    if(!m_cameraControl) return;
    auto pos = glm::vec2((float)x, (float)y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 0.8f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (m_cameraYaw < 0.0f) m_cameraYaw += 360.0f;
    if (m_cameraYaw > 360.0f) m_cameraYaw -= 360.0f;

    if (m_cameraPitch > 89.0f) m_cameraPitch = 89.0f;
    if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;
    
    m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y){
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            m_prevMousePos = glm:: vec2((float)x, (float)y);
            m_cameraControl = true;
        }
        else if (action == GLFW_RELEASE) {
            m_cameraControl = false;
        }
    }
}

bool Context::Init(){
    // pos.xyz, normal.xyz, texcoord.uv
    float vertices[] = { 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    };
    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };
    

    //vertex layout 을 활용
    m_vertexLayout = VertexLayout::Create();
    // buffer class 를 활용한 buffer gen -> binding 과정
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
    vertices,  sizeof(vertices));

    //vertex layout 을 활용
    m_vertexLayout->SetAttrib(0,3,GL_FLOAT, GL_FALSE, sizeof(float)*8, 0);
    m_vertexLayout->SetAttrib(1,3, GL_FLOAT,GL_FALSE, sizeof(float)*8, sizeof(float)*3);
    m_vertexLayout->SetAttrib(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*6);


    // buffer class 를 활용한 buffer gen -> binding 과정
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
    indices, sizeof(indices));


    m_simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
    if (!m_simpleProgram) return false;
    SPDLOG_INFO("Simple program id: {}", m_simpleProgram->Get());
    
    m_program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
    if (!m_program) return false;
    SPDLOG_INFO("Program id : {}" , m_program->Get());

    glClearColor(0.0f, 0.1f, 0.2f,0.0f);

    auto image = Image::Load("./IMG_sample/container.jpg");
    if(!image){
        return false;
    }
    SPDLOG_INFO("image: {}x{}, {} channels", 
        image->GetWidth(), image->GetHight(), image->GetChannelCount());
    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./IMG_sample/awesomeface.png");
    if(!image2)
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels", 
        image2->GetWidth(), image2->GetHight(), image2->GetChannelCount());
    m_texture2 = Texture::CreateFromImage(image2.get());

    m_material.diffuse = Texture::CreateFromImage(Image::Load("./IMG_sample/container2.png").get());
    

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    m_program->SetUniform("tex", 0);
    m_program->SetUniform("tex2", 1);

    return true;
}

glm::vec3 GetAttenuationCoeff(float distance){
    const auto linear_coeff = glm::vec4(
        8.4523112e-05, 4.4712582e+00, -1.8516388e+00, 3.3955811e+01
    );
    const auto quad_coeff = glm::vec4(
    -7.6103583e-04, 9.0120201e+00, -1.1618500e+01, 1.0000464e+02
    );

    float kc = 1.0f;
    float d = 1.0f / distance;
    auto dvec = glm::vec4(1.0f, d, d*d, d*d*d);
    float kl = glm::dot(linear_coeff, dvec);
    float kq = glm::dot(quad_coeff, dvec);

    return glm::vec3(kc, glm::max(kl, 0.0f), glm::max(kq*kq,0.0f));
}

void Context::Render(){
    if (ImGui::Begin("UI Window")){
        if(ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::DragFloat3("l.position", glm::value_ptr(m_light.position), 0.01f);
            ImGui::DragFloat("l.distance", &m_light.distance, 0.5f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(m_light.ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(m_light.diffuse));
            ImGui::ColorEdit3("lmspecular", glm::value_ptr(m_light.specular));
        }

        if(ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen)){   
            
            ImGui::ColorEdit3("m.specular", glm::value_ptr(m_material.specular));
            ImGui::DragFloat("m.shininess", &m_material.shininess, 1.0f, 1.0f, 256.0f);
        }
        ImGui::Checkbox("animation", &m_animation);
        if(ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor))){
            glClearColor(m_clearColor.x,m_clearColor.y,m_clearColor.z, m_clearColor.w);
        }
        ImGui::Separator();
        ImGui::DragFloat3("Camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat3("Camera Yaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat3("Camera Pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if(ImGui::Button("reset camera")){
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }
    }
    ImGui::End();
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    m_cameraFront = 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f,1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f))* 
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); 
        // homo coordi 인데 last value가 1이 아닌이유? -> 마지막이 0이면 vector , 1이면 점이 됨.
        // 그러면서 평행이동이 안된다.
    m_light.position = m_cameraPos;
    m_light.direction = m_cameraFront;

    auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 20.0f);
    auto view = glm::lookAt(
        m_cameraPos,
        m_cameraPos + m_cameraFront,
        m_cameraUp
    );

    // auto lightModelTransform =
    // glm::translate(glm::mat4(1.0), m_light.position) *
    // glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    
    // m_simpleProgram->Use();
    // m_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
    // m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);

    m_program->Use();
    m_program->SetUniform("viewPos", m_cameraPos);
    m_program->SetUniform("light.position", m_light.position);
    m_program->SetUniform("light.direction", m_light.direction);
    m_program->SetUniform("light.cutoff", glm::vec2(
        cosf(glm::radians(m_light.cutoff[0])),
        cosf(glm::radians(m_light.cutoff[0] + m_light.cutoff[1]))));
    m_program->SetUniform("light.attenuation",
        GetAttenuationCoeff(m_light.distance));
    m_program->SetUniform("light.diffuse", m_light.diffuse);
    m_program->SetUniform("light.ambient", m_light.ambient);
    m_program->SetUniform("light.specular", m_light.specular);
    m_program->SetUniform("material.diffuse", 0);
    m_program->SetUniform("material.specular", m_material.specular);
    m_program->SetUniform("material.shininess", m_material.shininess);
    
    glActiveTexture(GL_TEXTURE0);
    m_material.diffuse->Bind();
    for (size_t i = 0; i <cubePositions.size(); i++)
    {
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        auto angle = glm::radians((m_animation?  (float)glfwGetTime() : 0.0f) * 120.0f + 20.0f*(float)i);
        model = glm::rotate(model, 
            angle, glm::vec3(1.0f, 0.5f,0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        m_program->SetUniform("modelTransform", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}