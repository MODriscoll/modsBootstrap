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
		{

		}

	public:


	};

	// A wrapper for a glfw window
	class mdWindow
	{
	public:

		mdWindow(const mdWindowContext& context);
		~mdWindow();

	public:

		// Get access to the GLFW window being wrapped
		GLFWwindow* GetWindowContext() const { return m_Window; }

	private:

		// The glfw window
		GLFWwindow* m_Window;
	};
}