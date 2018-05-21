#include "Application\Application.h"

#include "IncludeGLFW.h"
#include "Application\Input.h"

#include <glm\common.hpp>

namespace mods
{
	Application::Application()
		: m_Window(nullptr)
		, m_AppStart(0.f)
		, m_FPS(0)
	{

	}

	bool Application::Run(int32 width, int32 height, const char* title)
	{
		// Initialize openGL
		if (!Initialize(width, height, title))
			return false;

		// Make sure the app starts successfully
		if (!Startup())
		{
			Cleanup();
			return false;
		}

		m_AppStart = (float)glfwGetTime();

		// Execute the game loop
		{
			double framestart = glfwGetTime();
			double deltatime = 0.f;
			double frametime = 0.f;

			int32 frames;

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
						frametime = glm::mod(frametime, 1.0);
					}
				}

				// Perform tick
				Tick((float)deltatime);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				// Draw frame
				Draw();

				// Display new frame to screen
				glfwSwapBuffers(m_Window);
			}
		}
		

		return false;
	}

	bool Application::Initialize(int32 width, int32 height, const char* title)
	{
		// Initialize openGL
		if (!glfwInit())
			return false;

		glfwSetErrorCallback(HandleGLError);

		// Get pointers to openGL functions
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			return false;
		}

		// Create our window
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Window);

		// Initializes data with this window
		glfwSetWindowUserPointer(m_Window, this);
		glViewport(0, 0, width, height);

		// Initializes callbacks
		glfwSetFramebufferSizeCallback(m_Window, HandleFramebufferResize);

		// Initialize input
		Input::Create();

		return true;
	}

	bool Application::Cleanup()
	{
		Input::Destroy();

		glfwDestroyWindow(m_Window);

		glfwTerminate();
	}
}