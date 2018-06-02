#include "Application\Application.h"

#include "IncludeGLFW.h"
#include "Application\Input.h"
#include "Application\Renderer.h"

#include <glm\common.hpp>

#include <cassert>
#include <iostream>

namespace mods
{
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

				Render->StartFrame();
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

		glfwSetErrorCallback(HandleGLError);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	void Application::HandleGLError(int32 error, const char* message)
	{
		std::cout << "[OpenGL] Error: Code=" << error << " Message: " << message << std::endl;
		assert(false);
	}

	void Application::HandleFramebufferResize(GLFWwindow* window, int32 width, int32 height)
	{
		glViewport(0, 0, width, height);
		Renderer::m_Singleton->m_Width = width;
		Renderer::m_Singleton->m_Height = height;
	}
}