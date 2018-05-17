#include "Window\Window.h"

#include "IncludeGLFW.h"

#include <iostream>

namespace mods
{
	Window::Window(const WindowConstructor& constructor)
		: m_Window(nullptr)
		, m_Input(nullptr)
	{
		m_Window = glfwCreateWindow(constructor.Width, constructor.Height, 
			constructor.Name.c_str(), nullptr, nullptr);

		if (!m_Window)
		{
			std::cout << "Error: Failed to create window." << std::endl;
			return;
		}

		glfwMakeContextCurrent(m_Window);

		// Setup our input
		m_Input = new Input();
		m_Input->Initialize(this);
	}

	Window::~Window()
	{
		if (m_Input)
		{
			m_Input->CleanUp();
			delete m_Input;
		}

		glfwDestroyWindow(m_Window);
	}
}