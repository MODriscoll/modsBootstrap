#pragma once

#include "InputTypes.h"

#include <array>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace mods
{
	class mdWindow;

	enum mdInputStatus : int
	{
		Release		= 0,
		Up			= 1,

		Press		= 2,
		Down		= 3
	};

	struct mdKey
	{
		mdKey()
		{
			Status = mdInputStatus::Up;
		}

		int Status;
	};

	// Handles the input for a single window. Should
	// not be used outside of the window wrapper
	// TODO: TIDY, feels very dirty right now
	// implement action and axis bindings
	class mdInput final	// Make virtual in future?
	{
		friend mdWindow;

	private:

		mdInput();
		mdInput(const mdInput& rhs) = delete;
		mdInput(mdInput&& rhs) = delete;

		~mdInput();

		mdInput& operator = (const mdInput& rhs) = delete;
		mdInput& operator = (mdInput&& rhs) = delete;

	public:

		// Get if the given key is down
		bool IsKeyDown(mdInputKey key) const;
		
		// Get if the given key is up
		bool IsKeyUp(mdInputKey key) const;

		// Get if the given key was recently pressed
		bool WasKeyPressed(mdInputKey key) const;

		// Get if the given key was recently released
		bool WasKeyReleased(mdInputKey key) const;

		// Get the position of the mouse
		void GetMousePosition(int& x, int& y) const;
		void GetMousePosition(float& x, float& y) const;

		// Get the recent scroll offset of the mouse wheel
		float GetMouseScroll() const;

	private:

		// Initializes this input to track the input of the given window
		bool Initialize(mdWindow* window);

		// Cleans up the association with the current window
		bool CleanUp();

		// Updates the polled inputs. This needs to be called
		// as certain events aren't continously polled
		void PollInputs();

		// Resets all input tracking
		void ClearRecords();

	private:

		// Event called by window when action has been performed on a key
		void HandleKeyEvent(mdInputKey key, mdInputAction action, mdInputMods mods);

		// Event called by window when mouse has been moved
		void HandleMouseMove(double x, double y);

		// Event called by window when mouse wheel has been scrolled
		void HandleMouseScroll(double offset);

	private:

		// Window this input is associated with
		mdWindow* m_Window;

		// Status of all keys (including mouse buttons)
		std::map<mdInputKey, mdKey> m_KeyStatus;

		// Keys updated during last polling
		std::vector<mdInputKey> m_UpdatedKeys;

		// Position of the mouse
		double m_MouseX, m_MouseY;

		// Scroll of the mouse wheel
		double m_MouseScroll;
	};
}