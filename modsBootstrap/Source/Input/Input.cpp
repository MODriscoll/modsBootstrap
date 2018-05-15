#include "Input\Input.h"

#include "IncludeGLFW.h"

#include <cassert>

namespace mods
{
	mdInput::mdInput()
	{
		mdInputEvent m;
		m == 5;
		GLFWwindow* window = glfwGetCurrentContext();

		auto keyevent = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Instance().KeyEvent(window, key, scancode, action, mods);
		};

		auto buttonevent = [](GLFWwindow* window, int button, int action, int mods)
		{
			Instance().MouseButtonEvent(window, button, action, mods);
		};

		auto moveevent = [](GLFWwindow* window, double xpos, double ypos)
		{
			Instance().MouseMoveEvent(window, xpos, ypos);
		};

		auto scrollevent = [](GLFWwindow* window, double xdelta, double ydelta)
		{
			Instance().MouseScrollEvent(window, xdelta, ydelta);
		};

		glfwSetKeyCallback(window, keyevent);
		glfwSetMouseButtonCallback(window, buttonevent);
		glfwSetCursorPosCallback(window, moveevent);
		glfwSetScrollCallback(window, scrollevent);
	}

	mdInput::~mdInput()
	{
	}

	mdInput& mdInput::Instance()
	{
		static mdInput Input;
		return Input;
	}

	void mdInput::ProcessStatus()
	{

	}

	bool mdInput::WasKeyPressed(mdInputKey key) const
	{
		
	}

	bool mdInput::IsKeyDown(mdInputKey key) const
	{
		return false;
	}

	bool mdInput::WasKeyReleased(mdInputKey key) const
	{
		return false;
	}

	void mdInput::GetMousePosition(float& x, float& y) const
	{
		x = (float)m_MouseX;
		y = (float)m_MouseY;
	}

	float mdInput::GetMouseScroll() const
	{
		return (float)m_MouseScroll;
	}

	void mdInput::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	}

	void mdInput::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
	{
	}

	void mdInput::MouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
	{
	}

	void mdInput::MouseScrollEvent(GLFWwindow* window, double xdelta, double ydelta)
	{
	}
}