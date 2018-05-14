#pragma once

#include "InputTypes.h"

#include <functional>
#include <map>
#include <string>

struct GLFWwindow;

namespace modsBootstrap
{
	// TODO:
	// Input Bindings

	class mdInput
	{
	public:

		mdInput(GLFWwindow* window);
		~mdInput();

	public:

		// Processes the existing inputs, updating the status of the input event
		void ProcessStatus();

	public:

		// Get if the given key has just been pressed
		bool WasKeyPressed(mdInputKey key) const;

		// Get if the given key is current held down
		bool IsKeyDown(mdInputKey key) const;

		// Get if the given key has just been released
		bool WasKeyReleased(mdInputKey key) const;

		// Get the screen position of the mouse
		void GetMousePosition(float& x, float& y) const;

		// Get the scroll delta of the mouse
		float GetMouseScroll() const;

	private:

		// Callback for when a key is pressed
		void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

		// Callback for when a mouse button is pressed
		void OnMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);

		// Callback for when mouse has moved
		void OnMouseMoveEvent(GLFWwindow* window, double xpos, double ypos);

		// Callback for when mouse wheel has been scrolled
		void OnMouseScrollEvent(GLFWwindow* window, double xdelta, double ydelta);

	private:

		// Status of all active keys
		std::map<mdInputKey, mdInputEvent> m_KeysStatus;

		// Status of all active mouse buttons
		std::map<mdInputKey, mdInputEvent> m_MouseButtonsStatus;

		// Location of mouse on the screen
		double m_MouseX, m_MouseY;

		// Delta of the mouse scrolling wheel
		double m_MouseScroll;
	};
}