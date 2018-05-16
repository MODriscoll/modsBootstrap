#pragma once

#include "Input\Input.h"

struct GLFWwindow;

namespace mods
{
	// Context for the creation of a new window
	struct mdWindowContext
	{
	public:

		mdWindowContext()
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
	class mdWindow
	{
	public:

		mdWindow(const mdWindowContext& context);
		virtual ~mdWindow();

	public:

		// Get access to the GLFW window being wrapped
		GLFWwindow* GetWindowContext() const { return m_Window; }

		// Get input associated with this window
		mdInput* GetInput() const { return m_Input; }

	private:

		// The glfw window
		GLFWwindow* m_Window;

		// Input for this window
		mdInput* m_Input;
	};
}