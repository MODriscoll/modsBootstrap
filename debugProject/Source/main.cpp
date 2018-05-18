#include <Rendering\Shaders\Shader.h>

#include <IncludeGLFW.h>

#include <Camera\Camera.h>
#include <Rendering\Textures\Texture.h>
#include <Rendering\Meshes\Model.h>

#include <glm\ext.hpp>

#include <iostream>

using int8 = char;
using int16 = short;
using int32 = int;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;

mods::Camera FlyCamera;

float deltaTime;
float lastTime;

// Callback for when the given window is resized
void framebuffer_size_callback(GLFWwindow* window, int32 width, int32 height)
{
	glViewport(0, 0, width, height);
}

bool bWIsDown = false;
bool bSIsDown = false;
bool bAIsDown = false;
bool bDIsDown = false;

bool bRightMouseIsDown = false;

void on_key_press(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool IsDown = (action == GLFW_PRESS || action == GLFW_REPEAT);

	switch (key)
	{
	case GLFW_KEY_W:
	{
		bWIsDown = IsDown;
		return;
	}
	case GLFW_KEY_S:
	{
		bSIsDown = IsDown;
		return;
	}
	case GLFW_KEY_A:
	{
		bAIsDown = IsDown;
		return;
	}
	case GLFW_KEY_D:
	{
		bDIsDown = IsDown;
		return;
	}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (FlyCamera.ProjectionMode == mods::eProjectionMode::Perspective)
			FlyCamera.ProjectionMode = mods::eProjectionMode::Orthographic;
		else
			FlyCamera.ProjectionMode = mods::eProjectionMode::Perspective;
	}

	/*std::cout << "(on_key_press) Key " << glfwGetKeyName(key, scancode) << " has been ";

	switch (action)
	{
		case GLFW_PRESS:
		{
			std::cout << "Pressed";
			break;
		}
		case GLFW_REPEAT:
		{
			std::cout << "Held Down";
			break;
		}
		case GLFW_RELEASE:
		{
			std::cout << "Released";
			break;
		}
		default:
		{
			std::cout << " ... I forget (Unknown)";
		}
	}

	std::cout << std::endl;*/
}

void on_mouse_enter(GLFWwindow* window, int entered)
{
	//if (entered)
	//{
	//	std::cout << "Mouse has entered window" << std::endl;
	//}
	//else
	//{
	//	std::cout << "Mouse has left window" << std::endl;
	//}
}

float lastX = FLT_MAX, lastY = FLT_MAX;

void on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
	float x = (float)xpos;
	float y = (float)ypos;

	if (!bRightMouseIsDown)
	{
		lastX = x;
		lastY = y;
		return;
	}

	if (lastX == FLT_MAX)
	{
		lastX = x;
		lastY = y;
	}

	float deltaX = x - lastX;
	float deltaY = lastY - y;

	deltaX *= 0.3f;
	deltaY *= 0.3f;

	glm::vec3 rotation = FlyCamera.GetRotation();
	rotation.x = glm::clamp(rotation.x + deltaY, -89.f, 89.f);
	rotation.y = glm::mod(rotation.y + deltaX, 360.f);
	FlyCamera.SetRotation(rotation);

	lastX = x;
	lastY = y;

	//std::cout << "Mouse position: X= " << xpos << ", Y= " << ypos << std::endl;
}

void on_mouse_press(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
		bRightMouseIsDown = (action == GLFW_PRESS);

	//std::cout << "(on_mouse_press) Mouse button has been ";

	//switch (action)
	//{
	//	case GLFW_PRESS:
	//	{
	//		std::cout << "Pressed";
	//		break;
	//	}
	//	case GLFW_REPEAT:
	//	{
	//		std::cout << "Held Down";
	//		break;
	//	}
	//	case GLFW_RELEASE:
	//	{
	//		std::cout << "Released";
	//		break;
	//	}
	//	default:
	//	{
	//		std::cout << " ... I forget (Unknown)";
	//	}
	//}

	//std::cout << std::endl;
}

void on_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << "Mouse was scrolled. XOffset= " << xoffset << ", YOffset = " << yoffset << std::endl;
}

void handle_error(int code, const char* description)
{
	std::cout << "Error code: " << code << "\nDescription:\n" << description << std::endl;
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

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
	{

		glfwSetErrorCallback(handle_error);

		// Initiate glfw window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif

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

		// Callback for when an error has occured
		glfwSetErrorCallback(handle_error);

		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		uint32 indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		glEnable(GL_DEPTH_TEST);

		//mdVertexBuffer box(vertices, 180, indices, 6);

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 180, vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		using namespace mods;

		ShaderProgramConstructor constructor;
		constructor.LoadShader(VertexShaderSource("Resources/Shaders/Vertex.vert"));
		constructor.LoadShader(FragmentShaderSource("Resources/Shaders/Fragment.frag"));
		std::shared_ptr<ShaderProgram> shader = constructor.Construct();

		ShaderProgramConstructor modelshaderconstructor;
		modelshaderconstructor.LoadShader(VertexShaderSource("Resources/Shaders/ModelVertex.vert"));
		modelshaderconstructor.LoadShader(FragmentShaderSource("Resources/Shaders/ModelFragment.frag"));
		std::shared_ptr<ShaderProgram> modelshader = constructor.Construct();

		mods::Texture texture("Resources/Textures/Container.jpg");

		mods::Model nanosuit("Resources/Models/Nanosuit/nanosuit.obj");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glfwSetKeyCallback(window, on_key_press);
		glfwSetCursorPosCallback(window, on_mouse_move);
		glfwSetMouseButtonCallback(window, on_mouse_press);
		glfwSetCursorEnterCallback(window, on_mouse_enter);
		glfwSetScrollCallback(window, on_mouse_scroll);

		FlyCamera.Position = glm::vec3(3.f);
		FlyCamera.LookAt(glm::vec3(0.f));

		deltaTime = 0.f;
		lastTime = (float)glfwGetTime();

		// Game loop, keep looping while window is active
		while (!glfwWindowShouldClose(window))
		{
			float timeNow = (float)glfwGetTime();
			deltaTime = timeNow - lastTime;
			lastTime = timeNow;

			// need to clear screen otherwise previous frames will still be visible
			glClearColor(0.2f, 0.3f, 0.3f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (bWIsDown)
				FlyCamera.Position += FlyCamera.GetHeading() * 5.f * deltaTime;
			if (bSIsDown)
				FlyCamera.Position -= FlyCamera.GetHeading() * 5.f * deltaTime;

			glm::vec3 right = glm::normalize(glm::cross(FlyCamera.GetHeading(), glm::vec3(0.f, 1.f, 0.f))) * 5.f * deltaTime;
			if (bAIsDown)
				FlyCamera.Position -= right;
			if (bDIsDown)
				FlyCamera.Position += right;

			glm::mat4 model(1.f);
			model = glm::translate(model, glm::vec3(2.f));

			glm::mat4 t = FlyCamera.GetProjectionView();

			shader->Bind();
			texture.Bind();
			shader->SetUniformValue("u_texture", 0);
			shader->SetUniformValue("model", model);
			shader->SetUniformValue("projection", FlyCamera.GetProjectionMatrix());
			shader->SetUniformValue("view", FlyCamera.GetViewMatrix());
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			texture.Unbind();
			shader->Unbind();

			modelshader->Bind();
			modelshader->SetUniformValue("model", glm::mat4(1.f));
			modelshader->SetUniformValue("projection", FlyCamera.GetProjectionMatrix());
			modelshader->SetUniformValue("view", FlyCamera.GetViewMatrix());

			nanosuit.Draw(*modelshader);
			modelshader->Unbind();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);

		glfwDestroyWindow(window);
	}
	// Always terminate before exiting
	glfwTerminate();

	return 0;
}