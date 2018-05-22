#include <Rendering\Shaders\Shader.h>

#include <IncludeGLFW.h>

#include <Camera\Camera.h>
#include <Rendering\Textures\Texture.h>
#include <Rendering\Meshes\Model.h>
#include <Rendering\Textures\Cubemap.h>
#include <Rendering\Fonts\Font.h>

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
	FlyCamera.SetOrthographicWidth((float)width);
	FlyCamera.SetOrthographicHeight((float)height);
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);

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

		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		unsigned int skyboxvao;
		glGenVertexArrays(1, &skyboxvao);
		glBindVertexArray(skyboxvao);

		unsigned int skyboxvbo;
		glGenBuffers(1, &skyboxvbo);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, skyboxVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnable(GL_DEPTH_TEST);
		
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		float quad[] = {
			-0.99f, -0.99f, 0.0f,
			0.99f, -0.99f, 0.0f,
			-0.99f, 0.99f, 0.0f,
			0.99f, 0.99f, 0.0f
		};
		uint32 quadindices[] =
		{
			0, 1, 2,
			1, 3, 2
		};

		mdVertexBuffer quadbuf(quad, 12, quadindices, 6);

		using namespace mods;

		ShaderProgramConstructor constructor;
		constructor.LoadShader(VertexShaderSource("Resources/Shaders/Vertex.vert"));
		constructor.LoadShader(FragmentShaderSource("Resources/Shaders/Fragment.frag"));
		std::shared_ptr<ShaderProgram> shader = constructor.Construct();

		ShaderProgramConstructor modelshaderconstructor;
		modelshaderconstructor.LoadShader(VertexShaderSource("Resources/Shaders/ModelVertex.vert"));
		modelshaderconstructor.LoadShader(FragmentShaderSource("Resources/Shaders/ModelFragment.frag"));
		std::shared_ptr<ShaderProgram> modelshader = modelshaderconstructor.Construct();

		ShaderProgramConstructor skyboxshaderconstructor;
		skyboxshaderconstructor.LoadShader(VertexShaderSource("Resources/Shaders/SkyboxVertex.vert"));
		skyboxshaderconstructor.LoadShader(FragmentShaderSource("Resources/Shaders/SkyboxFragment.frag"));
		std::shared_ptr<ShaderProgram> skyboxshader = skyboxshaderconstructor.Construct();

		ShaderProgramConstructor billboardconstructor;
		billboardconstructor.LoadShader(VertexShaderSource("Resources/Shaders/BillboardVertex.vert"));
		billboardconstructor.LoadShader(FragmentShaderSource("Resources/Shaders/BillboardFragment.frag"));
		std::shared_ptr<ShaderProgram> billboardshader = billboardconstructor.Construct();

		ShaderProgramConstructor fontconstructor;
		fontconstructor.LoadShader(VertexShaderSource("Resources/Shaders/FontVertex.vert"));
		fontconstructor.LoadShader(FragmentShaderSource("Resources/Shaders/FontFragment.frag"));
		std::shared_ptr<ShaderProgram> fontshader = fontconstructor.Construct();

		billboardshader->Bind();
		billboardshader->SetUniformValue("position", glm::vec3(2.f, 3.f, 2.f));
		billboardshader->SetUniformValue("size", glm::vec2(0.35f, 0.1f));
		billboardshader->Unbind();

		mods::Texture texture("Resources/Textures/Container.jpg");

		std::array<std::string, 6> skyboxpaths =
		{
			"Resources/Textures/hw_crater/craterlake_rt.tga",
			"Resources/Textures/hw_crater/craterlake_lf.tga",
			"Resources/Textures/hw_crater/craterlake_up.tga",
			"Resources/Textures/hw_crater/craterlake_dn.tga",
			"Resources/Textures/hw_crater/craterlake_bk.tga",
			"Resources/Textures/hw_crater/craterlake_ft.tga"
		};
		mods::Cubemap skybox(skyboxpaths);

		mods::Model nanosuit("Resources/Models/Nanosuit/nanosuit.obj");

		mods::Font consolas("Resources/Fonts/consolas.ttf");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glfwSetKeyCallback(window, on_key_press);
		glfwSetCursorPosCallback(window, on_mouse_move);
		glfwSetMouseButtonCallback(window, on_mouse_press);
		glfwSetCursorEnterCallback(window, on_mouse_enter);
		glfwSetScrollCallback(window, on_mouse_scroll);

		FlyCamera.Position = glm::vec3(3.f, 4.f, 3.f);
		FlyCamera.LookAt(glm::vec3(2.f, 3.f, 2.f));

		//glEnable(GL_SCISSOR_TEST);
		//glScissor(240, 100, 800, 500);

		deltaTime = 0.f;
		lastTime = (float)glfwGetTime();

		FlyCamera.SetOrthographicWidth(1280.f);
		FlyCamera.SetOrthographicHeight(720.f);
		
		float frametime = 0.f;

		int32 fps = 0;
		int32 frames = 0;

		// Game loop, keep looping while window is active
		while (!glfwWindowShouldClose(window))
		{
			float timeNow = (float)glfwGetTime();
			deltaTime = timeNow - lastTime;
			lastTime = timeNow;

			++frames;

			frametime += deltaTime;
			if (frametime >= 1.f)
			{
				fps = frames;
				frames = 0;

				frametime = glm::mod(frametime, 1.f);
			}

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

			//glScissor(240, 100, 800, 500);

			modelshader->Bind();
			modelshader->SetUniformValue("model", glm::mat4(1.f));
			modelshader->SetUniformValue("projection", FlyCamera.GetProjectionMatrix());
			modelshader->SetUniformValue("view", FlyCamera.GetViewMatrix());

			nanosuit.Draw(*modelshader);
			modelshader->Unbind();

			//glScissor(0, 0, 1280, 720);

			/*billboardshader->Bind();
			billboardshader->SetUniformValue("projection", FlyCamera.GetProjectionMatrix());
			billboardshader->SetUniformValue("view", FlyCamera.GetViewMatrix());
			quadbuf.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			quadbuf.Unbind();
			billboardshader->Unbind();*/

			glm::mat4 view = FlyCamera.GetViewMatrix();
			view[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);	// No position for sky box view projection

			glDepthFunc(GL_LEQUAL);
			skyboxshader->Bind();
			skybox.Bind();
			skyboxshader->SetUniformValue("skybox", 0);
			skyboxshader->SetUniformValue("projection", FlyCamera.GetProjectionMatrix());
			skyboxshader->SetUniformValue("view", view);
			glBindVertexArray(skyboxvao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			skybox.Unbind();
			skyboxshader->Unbind();
			glDepthFunc(GL_LESS);

			glEnable(GL_BLEND);
			fontshader->Bind();
			fontshader->SetUniformValue("projection", FlyCamera.GetProjectionMatrix(eProjectionMode::Orthographic));
			consolas.Draw(*fontshader, std::string("FPS: ") + std::to_string(fps), glm::vec2(10.f), glm::vec4(1.f));
			fontshader->Unbind();
			glDisable(GL_BLEND);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &skyboxvao);
		glDeleteBuffers(1, &skyboxvbo);

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);

		glfwDestroyWindow(window);
	}
	// Always terminate before exiting
	glfwTerminate();

	return 0;
}