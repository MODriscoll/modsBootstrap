#pragma once

#include "Types.h"
#include "Input\Input.h"

#include <functional>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace mods
{
	class Window;

	// Context for the construction of a new window
	struct WindowConstructor
	{
	public:

		WindowConstructor()
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

		Window(const WindowConstructor& constructor);
		virtual ~Window();

	public:

		// Called once per frame
		virtual void Tick(float DeltaTime) = 0;

		// Called to draw to the windows buffer
		virtual void Draw() = 0;

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