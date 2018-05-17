#include "Application\Application.h"

#include "IncludeGLFW.h"

namespace mods
{
	Application::Application()
		: m_FocusedWindow(nullptr)
	{
	}

	bool Application::Run()
	{
		
		m_AppStart = high_resolution_clock::now();
		m_FrameStart = high_resolution_clock::now();

		while (m_FocusedWindow)
		{
			// Get new delta time for this frame
			auto now = high_resolution_clock::now();
			auto delta = duration_cast<milliseconds>(now - m_FrameStart);
			m_FrameStart = now;
		}

		return false;
	}

	void Application::HandleError(int error, const char* description)
	{
	}

	bool Application::Initialize()
	{
		// First need to intialize GLFW
		if (!glfwInit())
			return false;

		// Initialize glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			return false;
		}

		return true;
	}

	void Application::Cleanup()
	{
	}
}