#pragma once

#include "Input\Input.h"

#include <functional>
#include <vector>

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

		// TODO: MORE OPTIONS http://www.glfw.org/docs/latest/window_guide.html
		
		// OTHER OPTIONS LIKE - Update if not focused
	};

	// A wrapper for a glfw window.
	class Window
	{
	public:

		Window();
		virtual ~Window();

	public:

		// Creates a new GLFW window assigned to this window.
		// No new window is created if a window already exists
		bool Initialize(const WindowContext& context);


	public:

		// Get access to the GLFW window being wrapped
		GLFWwindow* GetWindowContext() const { return m_Window; }

		// Get input associated with this window
		Input* GetInput() const { return m_Input; }

	protected:

		// The glfw window
		GLFWwindow* m_Window;

		// Input for this window
		Input* m_Input;

	private:

		// TODO: Callbacks
	};
}