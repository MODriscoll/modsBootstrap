#pragma once

#include "Input\Input.h"

struct GLFWwindow;

namespace mods
{
	// Context for the creation of a new window
	struct WindowContext
	{
	public:

		WindowContext()
			: Name("MyWindow")
			, Width(1280.f)
			, Height(720.f)
		{
			
		}

	public:

		// The name of the window
		std::string Name;

		// Width of the window
		float Width;

		// Height of the window
		float Height;

		// TODO: MORE OPTIONS
	};

	// A wrapper for a glfw window.
	class Window
	{
	public:

		Window(const WindowContext& context);
		virtual ~Window();

	public:

		// Get access to the GLFW window being wrapped
		GLFWwindow* GetWindowContext() const { return m_Window; }

		// Get input associated with this window
		Input* GetInput() const { return m_Input; }

	private:

		// The glfw window
		GLFWwindow* m_Window;

		// Input for this window
		Input* m_Input;
	};
}