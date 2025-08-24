//http://glew.sourceforge.net/
//사이트에서 GLEW 바이너리 버전다운로드. 필요 라이브러리 dependencies에 복사 후 설정
//http://glew.sourceforge.net/basic.html
//아래 예제 코드실행 확인 

//GLEW_STATIC Define 필요
#include <GL/glew.h> //glfw보다 먼저 include해야 함
#include <GLFW/glfw3.h>
#include <iostream>

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

	glfwTerminate();
	return 0;
}