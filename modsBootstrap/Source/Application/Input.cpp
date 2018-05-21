#include "Application\Input.h"

#include "IncludeGLFW.h"

#include <cassert>

namespace mods
{
	Input* Input::m_Singleton = nullptr;

	Input::Input()
		: m_MouseX(0.0)
		, m_MouseY(0.0)
		, m_OldMouseX(0.0)
		, m_OldMouseY(0.0)
		, m_MouseScroll(0.0)
	{

	}

	Input& Input::Instance()
	{
		assert(m_Singleton != nullptr);
		return *m_Singleton;
	}

	void Input::Create()
	{
		if (!m_Singleton)
		{
			m_Singleton = new Input();
			m_Singleton->Initialize();
		}
	}

	void Input::Destroy()
	{
		if (m_Singleton)
		{
			m_Singleton->Cleanup();
			delete m_Singleton;
		}
	}

	void Input::Update()
	{
		Input& input = Instance();
		input.Poll();
	}

	void Input::Initialize()
	{
		GLFWwindow* window = glfwGetCurrentContext();

		// Declare callbacks
		auto keyinput = [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)->void
		{
			Input& input = Instance();
			input.HandleKeyInputEvent((eInputKey)key, (eInputAction)action);
		};
		// TODO: charinput (see http://www.glfw.org/docs/latest/input_guide.html#input_char) gives codepoints for utf8 encoding
		auto buttoncallback = [](GLFWwindow* window, int32 button, int32 action, int32 modes)->void
		{
			Input& input = Instance();
			input.HandleKeyInputEvent((eInputKey)button, (eInputAction)action);
		};
		auto movecallback = [](GLFWwindow* window, double xpos, double ypos)->void
		{
			// Invert Y axis, as by default 0 = top
			int32 width, height;
			glfwGetWindowSize(window, &width, &height);

			Input& input = Instance();
			input.HandleMouseMoveEvent(xpos, (double)height - ypos);
		};
		auto scrollcallback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Input& input = Instance();
			input.HandleMouseScrollEvent(yoffset);
		};

		// Bind the callbacks
		glfwSetKeyCallback(window, keyinput);
		//glfwSetCharCallback(window, charinput);
		glfwSetMouseButtonCallback(window, buttoncallback);
		glfwSetCursorPosCallback(window, movecallback);
		glfwSetScrollCallback(window, scrollcallback);

		// Record mouse position now, to prevent initial jump from zero
		glfwGetCursorPos(window, &m_MouseX, &m_MouseY);
		m_OldMouseX = m_MouseX;
		m_OldMouseY = m_MouseY;
	}

	void Input::Cleanup()
	{
		GLFWwindow* window = glfwGetCurrentContext();

		glfwSetKeyCallback(window, nullptr);
		//glfwSetCharCallback(window, charinput);
		glfwSetMouseButtonCallback(window, nullptr);
		glfwSetCursorPosCallback(window, nullptr);
		glfwSetScrollCallback(window, nullptr);
	}

	void Input::HandleKeyInputEvent(eInputKey key, eInputAction action)
	{
		if (action == eInputAction::Hold)
		{
			m_Keys[key] = (1 << static_cast<byte>(eInputStatus::Down));
		}
		else
		{
			m_Keys[key] = (1 << static_cast<byte>(action));
			m_PolledKeys.push_back(key);
		}
	}

	void Input::HandleMouseMoveEvent(double xpos, double ypos)
	{
		m_MouseX = xpos;
		m_MouseY = ypos;
	}

	void Input::HandleMouseScrollEvent(double yoffset)
	{
		m_MouseScroll += yoffset;
	}

	void Input::Poll()
	{
		// Update status of polled keys
		{
			for (eInputKey key : m_PolledKeys)
				m_Keys[key] << 2;

			m_PolledKeys.clear();
		}

		// Update delta of mouse
		{
			m_OldMouseX = m_MouseX;
			m_OldMouseY = m_MouseY;
		}
	}

	bool Input::IsKeyDown(eInputKey key)
	{
		Input& input = Instance();
		return input.m_Keys[key] == static_cast<byte>(eInputStatus::IsDown);
	}

	bool Input::IsKeyUp(eInputKey key)
	{
		Input& input = Instance();
		return input.m_Keys[key] == static_cast<byte>(eInputStatus::IsUp);
	}

	bool Input::WasKeyPressed(eInputKey key)
	{
		Input& input = Instance();
		return input.m_Keys[key] == static_cast<byte>(eInputStatus::Press);
	}

	bool Input::WasKeyReleased(eInputKey key)
	{
		Input& input = Instance();
		return input.m_Keys[key] == static_cast<byte>(eInputStatus::Release);
	}

	float Input::GetMouseX()
	{
		Input& input = Instance();
		return (float)input.m_MouseX;
	}

	float Input::GetMouseY()
	{
		Input& input = Instance();
		return (float)input.m_MouseY;
	}

	float Input::GetMousePosition(float& x, float& y)
	{
		Input& input = Instance();
		x = (float)input.m_MouseX;
		y = (float)input.m_MouseY;
	}

	float Input::GetDeltaX()
	{
		Input& input = Instance();
		return (float)(input.m_MouseX - input.m_OldMouseX);
	}

	float Input::GetDeltaY()
	{
		Input& input = Instance();
		return (float)(input.m_MouseY - input.m_OldMouseY);
	}

	float Input::GetMouseDelta(float& x, float& y)
	{
		Input& input = Instance();
		x = (float)(input.m_MouseX - input.m_OldMouseX);
		y = (float)(input.m_MouseY - input.m_OldMouseY);
	}

	float Input::GetMouseScroll()
	{
		Input& input = Instance();
		return (float)input.m_MouseScroll;
	}
}
