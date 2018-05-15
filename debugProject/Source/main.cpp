#include "..\..\modsBootstrap\Include\Rendering\Shader\Shader.h"
#include "..\..\modsBootstrap\Include\Rendering\Shader\Shader.h"

#include <IncludeGLFW.h>

#include <iostream>

using int8 = char;
using int16 = short;
using int32 = int;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;

// Callback for when the given window is resized
void framebuffer_size_callback(GLFWwindow* window, int32 width, int32 height)
{
	glViewport(0, 0, width, height);
}

// Process input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

struct mdVertexBuffer
{
	mdVertexBuffer(float* const vertecies, int32 verticessize, uint32* indices, uint32 indicessize)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticessize, vertecies, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indicessize, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);

		// Unbind vertex array for buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~mdVertexBuffer()
	{
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void Bind()
	{
		glBindVertexArray(vao);
	}

	static void Unbind()
	{
		glBindVertexArray(0);
	}

private:

	uint32 vao;

	uint32 vbo;
	uint32 ebo;
};

const char *vertexshader = "#version 400 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentshader = "#version 400 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

struct mddShader
{
	mddShader(const char* vertex, const char* fragment)
	{
		uint32 vertexhandle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexhandle, 1, &vertex, nullptr);
		glCompileShader(vertexhandle);

		int32 success;
		glGetShaderiv(vertexhandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int32 length;
			glGetShaderiv(vertexhandle, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length];
			glGetShaderInfoLog(vertexhandle, 512, nullptr, log);
			std::cout << "Failed to compile vertex shader.\nLog:\n" << log << std::endl;
			delete[] log;
			glDeleteShader(vertexhandle);
			return;
		}

		uint32 fragmenthandle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmenthandle, 1, &fragment, nullptr);
		glCompileShader(fragmenthandle);
		glGetShaderiv(fragmenthandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int32 length;
			glGetShaderiv(vertexhandle, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length];
			glGetShaderInfoLog(fragmenthandle, 512, nullptr, log);
			std::cout << "Failed to compile fragment shader.\nLog:\n" << log << std::endl;
			delete[] log;
			glDeleteShader(fragmenthandle);
			return;
		}

		program = glCreateProgram();
		glAttachShader(program, vertexhandle);
		glAttachShader(program, fragmenthandle);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			int32 length;
			glGetShaderiv(vertexhandle, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length];
			glGetProgramInfoLog(program, 512, nullptr, log);
			std::cout << "Failed to link program.\nLog:\n" << log << std::endl;
			delete[] log;
			glDeleteProgram(program);
		}
		
		// TODO:
		// Don't detach shaders for debugging purposes
		// see - https://www.youtube.com/watch?v=71BLZwRGUJE&index=6&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2 at 12:50
		glDetachShader(program, vertexhandle);
		glDetachShader(program, fragmenthandle);
		glDeleteShader(vertexhandle);
		glDeleteShader(fragmenthandle);
	}

	~mddShader()
	{
		glDeleteProgram(program);
	}

	void Bind()
	{
		glUseProgram(program);
	}

	void Unbind()
	{
		glUseProgram(0);
	}

private:

	uint32 program;
};

int main()
{
	// Initiate glfw
	if (!glfwInit())
		return -1;

	// Initiate glfw window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "modsBootstrap", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;

		// Must terminate if init was successfull
		glfwTerminate();
		return -1;	// GLFW initialize failed
	}

	glfwMakeContextCurrent(window);

	// Have to initialize glad before gl fuctions can be used
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;

		// Must terminate if init was successfull
		glfwTerminate();
		return -2;	// Glad failed to find function pointers
	}

	// Size of the viewport opengl will render
	glViewport(0, 0, 1280, 720);

	// Callback for when window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	uint32 indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	mdVertexBuffer box(vertices, 12, indices, 6);
	mddShader shader(vertexshader, fragmentshader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game loop, keep looping while window is active
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// need to clear screen otherwise previous frames will still be visible
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		box.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		box.Bind();
		shader.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Always terminate before exiting
	glfwTerminate();

	return 0;
}