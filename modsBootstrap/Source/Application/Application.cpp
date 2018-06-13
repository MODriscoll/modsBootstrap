#include "Application\Application.h"

#include "IncludeGLFW.h"
#include "Application\Input.h"
#include "Application\Renderer.h"

#include <glm\common.hpp>

#include <cassert>
#include <iostream>

namespace mods
{
	#if _DEBUG

	void APIENTRY HandleOpenGLDebugOutput(
		uint32 source,
		uint32 type,
		uint32 id,
		uint32 severity,
		int32 length,
		const char* message,
		const void* userParam)
	{
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 1281) 
			return;

		std::cout << "\n[OpenGL]\nDebug Output: " << id << "\nMessage: " << message << std::endl;

		switch (source)
		{
			case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << '\n';

		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << '\n';

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: High"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: Medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: Low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification"; break;
		} std::cout << '\n' << std::endl;
	}

	#endif

	Application::Application()
		: m_Window(nullptr)
		, m_AppStart(0.f)
		, m_FPS(0)
	{

	}

	int32 Application::Run(int32 width, int32 height, const char* title)
	{
		// Initialize base application
		if (!Initialize(width, height, title))
			return -1;

		// Make sure the app starts successfully
		if (!Startup())
		{
			Cleanup();
			return -2;
		}

		m_AppStart = (float)glfwGetTime();

		// Execute the game loop
		{
			double framestart = glfwGetTime();
			double deltatime = 0.f;
			double frametime = 0.f;

			int32 frames = 0;

			Renderer* Render = Renderer::m_Singleton;

			while (!glfwWindowShouldClose(m_Window))
			{
				// Update events
				{
					Input::Update();
					glfwPollEvents();
				}

				// Skip if iconified
				if (glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED))
					continue;

				// Update time
				{
					// Get new delta time
					double frameend = glfwGetTime();
					deltatime = frameend - framestart;
					framestart = frameend;

					// Track fps
					++frames;
					if ((frametime += deltatime) >= 1.0)
					{
						m_FPS = frames;
						frames = 0;
						frametime -= -1.0;
					}
				}

				// Perform tick
				Tick((float)deltatime);

				Render->StartFrame((float)glfwGetTime());
				Render->StartGeometryPass();

				// Draw geometry for this frame
				Draw();

				Render->StartLightingPass();
				Render->StartPostProcessPass();
				Render->EndFrame();

				// Display new frame to screen
				glfwSwapBuffers(m_Window);
			}
		}
		
		// Be sure to record if an error has happened
		int32 error = 0;
		if (!Shutdown())
			error = 3;

		if (!Cleanup())
			error = 4;

		return error;
	}

	int32 Application::GetWindowWidth() const
	{
		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		return width;
	}

	int32 Application::GetWindowHeight() const
	{
		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		return height;
	}

	void Application::GetWindowsSize(int32& width, int32& height) const
	{
		glfwGetWindowSize(m_Window, &width, &height);
	}

	float Application::GetExecutuinTime() const
	{
		double now = glfwGetTime();
		return (float)now - m_AppStart;
	}

	void Application::Stop()
	{
		glfwSetWindowShouldClose(m_Window, 1);
	}

	bool Application::Initialize(int32 width, int32 height, const char* title)
	{
		// Initialize openGL
		if (!glfwInit())
			return false;

		glfwSetErrorCallback(HandleGLFWError);

		// Setting 4.3 to utilize debbuger
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Till resize option has been implemented
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		#if _DEBUG

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		#endif

		// Create our window
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Window);

		// Get pointers to openGL functions
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			return false;
		}

		#if _DEBUG

		int32 flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT &&
			GLAD_GL_VERSION_4_3) // ARB_debug_output exists but is not included in versions below 4.3
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(HandleOpenGLDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

			std::cout << "Debug Output Initialised" << std::endl;
		}

		#endif

		#if _DEBUG

		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		int32 extensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
		std::cout << "Number of Extensions: " << extensions << '\n' << std::endl;

		#endif

		// Initializes data with this window
		glfwSetWindowUserPointer(m_Window, this);
		glViewport(0, 0, width, height);

		// Initializes callbacks
		glfwSetFramebufferSizeCallback(m_Window, HandleFramebufferResize);

		// Initialize input
		Input::Create();

		// Initialize renderer
		Renderer::Create(width, height);

		return true;
	}

	bool Application::Cleanup()
	{
		Renderer::Destroy();
		Input::Destroy();

		glfwDestroyWindow(m_Window);

		glfwTerminate();

		return true;
	}

	void Application::HandleGLFWError(int32 error, const char* message)
	{
		std::cout << "\n[GLFW]\nError: " << error << "\nMessage: " << message << std::endl;
	}

	void Application::HandleFramebufferResize(GLFWwindow* window, int32 width, int32 height)
	{
		//glViewport(0, 0, width, height);
		Renderer::m_Singleton->m_Width = width;
		Renderer::m_Singleton->m_Height = height;
	}
}