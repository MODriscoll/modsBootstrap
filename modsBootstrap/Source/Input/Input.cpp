#include "Input\Input.h"

#include "IncludeGLFW.h"
#include "Window\Window.h"

#include <cassert>
#include <iostream>

namespace mods
{
	mdInput::mdInput()
		: m_Window(nullptr)
		, m_MouseX(0.0)
		, m_MouseY(0.0)
		, m_MouseScroll(0.0)
	{

	}

	mdInput::~mdInput()
	{
		CleanUp();
	}

	bool mdInput::IsKeyDown(mdInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const mdKey& status = it->second;
			return status.Status == mdInputStatus::Press || status.Status == mdInputStatus::Down;
		}
		
		// Key has not been found, this most
		// key has not been pressed yet
		return false;
	}

	bool mdInput::IsKeyUp(mdInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const mdKey& status = it->second;
			return status.Status == mdInputStatus::Release || status.Status == mdInputStatus::Up;
		}

		// Key has not been found, this most
		// key has not been pressed yet
		return true;
	}

	bool mdInput::WasKeyPressed(mdInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const mdKey& status = it->second;
			return status.Status == mdInputStatus::Press;
		}

		// Key has not been found, this most
		// key has not been pressed yet
		return false;
	}

	bool mdInput::WasKeyReleased(mdInputKey key) const
	{
		auto it = m_KeyStatus.find(key);
		if (it == m_KeyStatus.cend())
		{
			const mdKey& status = it->second;
			return status.Status == mdInputStatus::Release;
		}

		// Key has not been found, this most
		// key has not been pressed yet
		return false;
	}

	void mdInput::GetMousePosition(int& x, int& y) const
	{
		x = (int)m_MouseX;
		y = (int)m_MouseY;
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

	bool mdInput::Initialize(mdWindow* wrapper)
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

			mdWindow* wrapper = static_cast<mdWindow*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			wrapper->GetInput()->HandleKeyEvent((mdInputKey)key, (mdInputAction)action, (mdInputMods)mods);
		};
		auto ButtonCallback = [](GLFWwindow* window, int button, int action, int mods)->void
		{
			mdWindow* wrapper = static_cast<mdWindow*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			wrapper->GetInput()->HandleKeyEvent((mdInputKey)button, (mdInputAction)action, (mdInputMods)mods);
		};
		auto MoveCallback = [](GLFWwindow* window, double xpos, double ypos)->void
		{
			mdWindow* wrapper = static_cast<mdWindow*>(glfwGetWindowUserPointer(window));
			assert(wrapper != nullptr);

			// Convert Y origin to bottom of screen instead of top
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			ypos = (double)height - ypos;

			wrapper->GetInput()->HandleMouseMove(xpos, ypos);
		};
		auto ScrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)->void
		{
			mdWindow* wrapper = static_cast<mdWindow*>(glfwGetWindowUserPointer(window));
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

	bool mdInput::CleanUp()
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

	void mdInput::PollInputs()
	{
		// Update the polled inputs
		for (const mdInputKey& key : m_UpdatedKeys)
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

	void mdInput::ClearRecords()
	{
		m_KeyStatus.clear();
		m_UpdatedKeys.clear();
		m_MouseX = m_MouseY = 0.0;
		m_MouseScroll = 0.0;
	}

	void mdInput::HandleKeyEvent(mdInputKey key, mdInputAction action, mdInputMods mods)
	{
		mdKey& status = m_KeyStatus[key];

		switch (action)
		{
			case mdInputAction::Press:
			{
				status.Status = mdInputStatus::Press;
				break;
			}
			case mdInputAction::Release:
			{
				status.Status = mdInputStatus::Release;
				break;
			}
			// Any action after press or released don't need to be further polled
			case mdInputAction::Repeat:
			{
				status.Status = mdInputStatus::Down;
				return;
			}
			default:				
			{
				return;
			}
		}

		m_UpdatedKeys.push_back(key);
	}

	void mdInput::HandleMouseMove(double x, double y)
	{
		m_MouseX = x;
		m_MouseY = y;
	}

	void mdInput::HandleMouseScroll(double offset)
	{
		m_MouseScroll += offset;
	}
}