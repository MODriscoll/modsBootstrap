#include "Input\Input.h"

#include "IncludeGLFW.h"
#include "Window\Window.h"

#include <cassert>
#include <iostream>

namespace mods
{
	Input::Input()
		: m_Window(nullptr)
		, m_MouseX(0.0)
		, m_MouseY(0.0)
		, m_MouseScroll(0.0)
	{

	}

	Input::~Input()
	{
		CleanUp();
	}

	bool Input::IsKeyDown(eInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const Key& status = it->second;
			return status.Status == eInputStatus::Press || status.Status == eInputStatus::Down;
		}
		
		// Key has not been found, this most
		// key has not been pressed yet
		return false;
	}

	bool Input::IsKeyUp(eInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const Key& status = it->second;
			return status.Status == eInputStatus::Release || status.Status == eInputStatus::Up;
		}

		// Key has not been found, this most
		// key has not been pressed yet
		return true;
	}

	bool Input::WasKeyPressed(eInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const Key& status = it->second;
			return status.Status == eInputStatus::Press;
		}

		// Key has not been found, this most
		// key has not been pressed yet
		return false;
	}

	bool Input::WasKeyReleased(eInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const Key& status = it->second;
			return status.Status == eInputStatus::Release;
		}

		// Key has not been found, this most
		// key has not been pressed yet
		return false;
	}

	void Input::GetMousePosition(int& x, int& y) const
	{
		x = (int)m_MouseX;
		y = (int)m_MouseY;
	}

	void Input::GetMousePosition(float& x, float& y) const
	{
		x = (float)m_MouseX;
		y = (float)m_MouseY;
	}

	float Input::GetMouseScroll() const
	{
		return (float)m_MouseScroll;
	}

	bool Input::Initialize(Window* wrapper)
	{
		if (wrapper == m_Window)
			return true;

		// Drop any references to our current window
		if (m_Window)
		{
			if (!CleanUp())
			{
				std::cout << "Error: Unable to associate input with window."
					" Failed to clean up previous associated window" << std::endl;
				return false;
			}
		}

		m_Window = wrapper;

		// Initialize the callbacks
		auto KeyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)->void
		{
			if (key == GLFW_KEY_UNKNOWN)
			{
				std::cout << "Unknown key recieved" << std::endl;
				return;
			}

			Window* wrapper = static_cast<Window*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			wrapper->GetInput()->HandleKeyEvent((eInputKey)key, (eInputAction)action, (eInputMods)mods);
		};
		auto ButtonCallback = [](GLFWwindow* window, int button, int action, int mods)->void
		{
			Window* wrapper = static_cast<Window*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			wrapper->GetInput()->HandleKeyEvent((eInputKey)button, (eInputAction)action, (eInputMods)mods);
		};
		auto MoveCallback = [](GLFWwindow* window, double xpos, double ypos)->void
		{
			Window* wrapper = static_cast<Window*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			// Convert Y origin to bottom of screen instead of top
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			ypos = (double)height - ypos;

			wrapper->GetInput()->HandleMouseMove(xpos, ypos);
		};
		auto ScrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)->void
		{
			Window* wrapper = static_cast<Window*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			wrapper->GetInput()->HandleMouseScroll(yoffset);
		};

		// Set the callbacks
		GLFWwindow* window = wrapper->GetWindowContext();
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, ButtonCallback);
		glfwSetCursorPosCallback(window, MoveCallback);
		glfwSetScrollCallback(window, ScrollCallback);

		return true;
	}

	bool Input::CleanUp()
	{
		if (!m_Window)
			return true;

		GLFWwindow* window = m_Window->GetWindowContext();

		// Drop all callbacks associated with this 
		glfwSetKeyCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		glfwSetCursorPosCallback(window, nullptr);
		glfwSetScrollCallback(window, nullptr);

		ClearRecords();

		m_Window = nullptr;

		return true;
	}

	void Input::PollInputs()
	{
		// Update the polled inputs
		for (const eInputKey& key : m_UpdatedKeys)
		{
			// The input status is designed so that
			// adding one to either press or release
			// update its to up or down
			m_KeyStatus[key].Status += 1;
		}

		m_UpdatedKeys.clear();

		// Need to reset most recent mouse scroll
		m_MouseScroll = 0.0;
	}

	void Input::ClearRecords()
	{
		m_KeyStatus.clear();
		m_UpdatedKeys.clear();
		m_MouseX = m_MouseY = 0.0;
		m_MouseScroll = 0.0;
	}

	void Input::HandleKeyEvent(eInputKey key, eInputAction action, eInputMods mods)
	{
		Key& status = m_KeyStatus[key];

		switch (action)
		{
			case eInputAction::Press:
			{
				status.Status = eInputStatus::Press;
				break;
			}
			case eInputAction::Release:
			{
				status.Status = eInputStatus::Release;
				break;
			}
			// Any action after press or released don't need to be further polled
			case eInputAction::Repeat:
			{
				status.Status = eInputStatus::Down;
				return;
			}
			default:				
			{
				return;
			}
		}

		m_UpdatedKeys.push_back(key);
	}

	void Input::HandleMouseMove(double x, double y)
	{
		m_MouseX = x;
		m_MouseY = y;
	}

	void Input::HandleMouseScroll(double offset)
	{
		m_MouseScroll += offset;
	}
}