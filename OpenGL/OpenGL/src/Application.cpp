//http://glew.sourceforge.net/
//사이트에서 GLEW 바이너리 버전다운로드. 필요 라이브러리 dependencies에 복사 후 설정
//http://glew.sourceforge.net/basic.html
//아래 예제 코드실행 확인 

//GLEW_STATIC Define 필요
#include <GL/glew.h> //glfw보다 먼저 include해야 함
#include <GLFW/glfw3.h>
#include <iostream>
static unsigned int Compileshader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type); //셰이더 객체 생성
	const char* src = source.c_str();
	glShaderSource(id, // 셰이더의 소스 코드를 명시 ,소스 코드를 명시할 셰이더 객체의 ID
		1, // 몇개의 소스 코드를 명시할 것인지?
		&src, // 실제 소스 코드가 들어있는 문자열의 주소값
		nullptr); // 해당 문자열 전체를 사용할 경우 nullptr 입력, 아니라면 길이를 명시해줘야함
	glCompileShader(id); // id 에 해당하는 셰이더 컴파일

	// 여기서부터는 Error Handling (없으면 힘들다고 함)
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length; // 결국엔 로그를 읽어오기 위한 과정
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // log 의 길이를 얻어옴
		char* message = (char*)alloca(length * sizeof(char)); // stack에 동적할당
		glGetShaderInfoLog(id, length, &length, message); // 길이만큼 log를 얻어옴
		std::cout << "Shader Compile False" << (type == GL_VERTEX_SHADER ? "vertex" : "Fragment") << "\n";
		std::cout << message << "\n";
		glDeleteShader(id);
		return 0;
	}
	return id;
}
static unsigned int Createshader(const std::string& vertexShader, const std::string& fragShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = Compileshader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = Compileshader(GL_FRAGMENT_SHADER, fragShader);

	// 컴파일 된 쎼이더 코드를 program에 추가하고 링크
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// 셰이더 프로그램을 생성했으므로 vs, fs 개별 프로그램은 더이상 필요가 없어진다. -> 삭제
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

	// 결국 vs, fs로 program을 하나 컴파일하고 사용된 두개의 셰이더는 종료시키고 만들어진 프로그램만 리턴함
}
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	// GPU가 사용할 윈도우를 지정해주어야 함.
	// *추가 : 하나의 프로그램이 여러 창을 가질 수 있고 , 각 창 마다 다른 OpenGL 상태를 가질 수도 있음.
	glfwMakeContextCurrent(window);

	// glfwMakeContextCurrent가 호출된 후에 glewInit이 수행되어야 함
	//glew(Init) -> OpenGL을 사용하기 위한 라이브러리
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	std::cout << glGetString(GL_VERSION) << std::endl; //내 플랫폼의 GL_Version 출력해보기

	// 이런식의 정보는 Ram 에 저장됨.
	float position[6] = {
		-0.5f,-0.5f,
		 0.0f, 0.5f,
		 0.5f,-0.5f
	};
	// 데이터를 가지고 이미지를 만드는 곳은 GPU에서 진행하게 됨.
	// 따라서 GPU VRAM으로의 데이터 전환이 필요하다.
	
	//GlgenBuffers(몇개의 버퍼를 생성할 지, 생성된 ID를 저장할 변수의 주소)
	//GlBindBuffer(GL_ARRAY_BUFFER,bufferID);
	//GL_ARRAY_BUFFER -> 이 퍼버는 vertex 데이터용라고 OpenGL 에게 설명
	// 다른 종류 예시 : GL_ELEMENT_ARRAY_BUFFER(인덱스용), GL_UNIFORM_BURRFER(uniform용) 등
	//GL_STATIC_DRAW -> 이 데이터는 한 번 설정하고 여러 번 사용할 것
	//GL_DYNAMIC_DRAW -> 자주 변경될 것
	//GL_STREAM_DRAW -> 한번 사용하고 버릴 것
	unsigned int bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID); // bind 는 activate의 역할을 함 이렇게 하면 buffer 데이터의 메타 정보는 필요하지 않게 된다.
	glBufferData(GL_ARRAY_BUFFER, // 실제 CPU->GPU
				6 * sizeof(float),
				position,
				GL_STATIC_DRAW);

	// 데이터 해석 법?
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0,
		2, // 하나의 vertex 에 몇개의 데이터를 넘기는지 ex) 2차원 좌표는 두개의 값이 필요
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 2, // float array 에서 read 에 대한 offset 
		0); // 0번째 index 부터 시작할 것
	
	// if 이전의 데이터를 3D 로 확장한다면?
	//float position[9] = {
	//	-0.5f,-0.5f,0.0f,
	//	 0.0f, 0.5f,0.0f,
	//	 0.5f,-0.5f,0.0f,
	//};
	//unsigned int bufferID;
	//glGenBuffers(1, &bufferID);
	//glBindBuffer(GL_ARRAY_BUFFER, bufferID); 
	//glBufferData(GL_ARRAY_BUFFER, 
	//	9 * sizeof(float),
	//	position,
	//	GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0,
	//	3, 
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(float) * 3,  
	//	0); 

	// Shader
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0 ) in vec4 position;" // 여기있는 location = 0 이 읽어와야할 버퍼? 이다.
		"\n"
		"void main()\n"
		"{\n"
		"   gl_Position = position;\n"
		"}\n";

	std::string fragShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		"   color = vec4(1.0,1.0,0.0,1.0);\n"
		"}\n";
	// shader의 인덱스를 받아둠 ( GPU에 있는 쉐이더의 주소를 CPU에 저장)
	unsigned int shader = Createshader(vertexShader, fragShader);
	glUseProgram(shader); // activate == bine랑 개념이 비슷함 

	// 많은 작업을 앞으로 이 While loop 문 안에서 진행하게 된다.
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// glfw 와 gl 라이브러리는 다르다.
		//rendering에 관련된 코드는 gl을 사용하게 될 것.

		/* Render here */
		// 반복문 1회는 하나의 프레임을 만들게 되는데
		// 때문에 이전 프레임의 정보를 지워줘야함.
		glClear(GL_COLOR_BUFFER_BIT);

		// 삼각형 그리는 Legacy 코드 추가
		// 다른 코드를 참조할 때 Glbegin , glend가 사용된 코드라면 참조하지 않는 것이 좋음
		// -> 옛날 코드들
		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();*/
		
		//glDrawArrays(GL_TRIANGLES, 0, 3)
		//parameter1 = 그릴 방식 (GL_TRIANGLES) 3개씩 묶어서 삼각형
		//parameter2 = 시작 vertex 인덱스
		//parameter3 = 그릴 vertex 개수
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw call -> 데이터를 넘겼다. 하지만 Shader로 어떻게 그릴 것인지에
		// 대해 설정해줘야함.
		/* Swap front and back buffers */	
		glfwSwapBuffers(window);
		// Swap buffer? 
		// 일종의 버퍼에도 순서가 존재하게 된다. (현재와 예비?) 
		// 예비 버퍼의 데이터를 현재에 전송?

		/* Poll for and process events */
		glfwPollEvents();
	}
	// 셰이더는 다쓰면 정리해줄것
	glDeleteShader(shader);
	glfwTerminate();
	return 0;
}