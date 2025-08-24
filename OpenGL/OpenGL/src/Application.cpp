//http://glew.sourceforge.net/
//����Ʈ���� GLEW ���̳ʸ� �����ٿ�ε�. �ʿ� ���̺귯�� dependencies�� ���� �� ����
//http://glew.sourceforge.net/basic.html
//�Ʒ� ���� �ڵ���� Ȯ�� 

//GLEW_STATIC Define �ʿ�
#include <GL/glew.h> //glfw���� ���� include�ؾ� ��
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
	// GPU�� ����� �����츦 �������־�� ��.
	// *�߰� : �ϳ��� ���α׷��� ���� â�� ���� �� �ְ� , �� â ���� �ٸ� OpenGL ���¸� ���� ���� ����.
	glfwMakeContextCurrent(window);

	// glfwMakeContextCurrent�� ȣ��� �Ŀ� glewInit�� ����Ǿ�� ��
	//glew(Init) -> OpenGL�� ����ϱ� ���� ���̺귯��
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	std::cout << glGetString(GL_VERSION) << std::endl; //�� �÷����� GL_Version ����غ���

	// �̷����� ������ Ram �� �����.
	float position[6] = {
		-0.5f,-0.5f,
		 0.0f, 0.5f,
		 0.5f,-0.5f
	};
	// �����͸� ������ �̹����� ����� ���� GPU���� �����ϰ� ��.
	// ���� GPU VRAM������ ������ ��ȯ�� �ʿ��ϴ�.
	
	//GlgenBuffers(��� ���۸� ������ ��, ������ ID�� ������ ������ �ּ�)
	//GlBindBuffer(GL_ARRAY_BUFFER,bufferID);
	//GL_ARRAY_BUFFER -> �� �۹��� vertex �����Ϳ��� OpenGL ���� ����
	// �ٸ� ���� ���� : GL_ELEMENT_ARRAY_BUFFER(�ε�����), GL_UNIFORM_BURRFER(uniform��) ��
	//GL_STATIC_DRAW -> �� �����ʹ� �� �� �����ϰ� ���� �� ����� ��
	//GL_DYNAMIC_DRAW -> ���� ����� ��
	//GL_STREAM_DRAW -> �ѹ� ����ϰ� ���� ��
	unsigned int bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID); // bind �� activate�� ������ �� �̷��� �ϸ� buffer �������� ��Ÿ ������ �ʿ����� �ʰ� �ȴ�.
	glBufferData(GL_ARRAY_BUFFER, // ���� CPU->GPU
				6 * sizeof(float),
				position,
				GL_STATIC_DRAW);

	// ���� �۾��� ������ �� While loop �� �ȿ��� �����ϰ� �ȴ�.
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// glfw �� gl ���̺귯���� �ٸ���.
		//rendering�� ���õ� �ڵ�� gl�� ����ϰ� �� ��.

		/* Render here */
		// �ݺ��� 1ȸ�� �ϳ��� �������� ����� �Ǵµ�
		// ������ ���� �������� ������ ���������.
		glClear(GL_COLOR_BUFFER_BIT);

		// �ﰢ�� �׸��� Legacy �ڵ� �߰�
		// �ٸ� �ڵ带 ������ �� Glbegin , glend�� ���� �ڵ��� �������� �ʴ� ���� ����
		// -> ���� �ڵ��
		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();*/
		
		//glDrawArrays(GL_TRIANGLES, 0, 3)
		//parameter1 = �׸� ��� (GL_TRIANGLES) 3���� ��� �ﰢ��
		//parameter2 = ���� vertex �ε���
		//parameter3 = �׸� vertex ����
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw call -> �����͸� �Ѱ��. ������ Shader�� ��� �׸� ��������
		// ���� �����������.
		/* Swap front and back buffers */	
		glfwSwapBuffers(window);
		// Swap buffer? 
		// ������ ���ۿ��� ������ �����ϰ� �ȴ�. (����� ����?) 
		// ���� ������ �����͸� ���翡 ����?

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}