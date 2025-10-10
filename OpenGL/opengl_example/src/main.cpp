#include "context.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window,
    int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc , const char** argv){
    SPDLOG_INFO("start program");
    
    SPDLOG_INFO("Initialize glfw");
    if(!glfwInit()){
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("Failed to  initialize glfw : {}", description);
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
      nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    // window 생성 후 함께 생성된 opengl context의 사용을 명시적으로 언급
    glfwMakeContextCurrent(window);

    // glad를 활용한 OpenGL 함수 로딩 -> 이 시점부터 opengl 을 사용할 수 있게 되는거임
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    // opengl 의 가장 기초 함수를 사용하면서 opengl 버전을 확인
    auto glVersion = glGetString(GL_VERSION);
    // 강의에서와 다르게 glGetString 함수가 const char* 를 return 하는데
    // 최신 버전의 spdlog에서는 직접 formating 허용 x 
    // 그래서 아래와 같이 로그 출력해야함.
    SPDLOG_INFO("OpenGL context version: {}", 
    reinterpret_cast<const char*>(glVersion));
    
    auto context = Context::Create();
    if(!context){
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    ShaderPtr vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());
    
    auto program = Program::Create({fragmentShader, vertexShader});
    SPDLOG_INFO("program id: {}", program->Get());
    
    // 최초의 frame 정보를 호출하지 않기 때문에 최소 로그를 위한 실행
    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    // 당연하게도 opengl 이 로드가 된 이후에 glfw 를 사용하면서 window 크기에 대한 event 수집
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        context->Render();
        glfwSwapBuffers(window);
    }
    context.reset();
    //context = nullptr;
    glfwTerminate();
    return 0;
}