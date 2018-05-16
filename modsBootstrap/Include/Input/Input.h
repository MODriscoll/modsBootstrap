#pragma once

#include "InputTypes.h"

#include <array>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace mods
{
	class Window;

	enum eInputStatus : int
	{
		Release		= 0,
		Up			= 1,

		Press		= 2,
		Down		= 3
	};

	struct Key
	{
		Key()
		{
			Status = eInputStatus::Up;
		}

		int Status;
	};

	// Handles the input for a single window. Should
	// not be used outside of the window wrapper
	// TODO: TIDY, feels very dirty right now
	// implement action and axis bindings
	class Input final	// Make virtual in future?
	{
		friend Window;

	private:

		Input();
		Input(const Input& rhs) = delete;
		Input(Input&& rhs) = delete;

		~Input();

		Input& operator = (const Input& rhs) = delete;
		Input& operator = (Input&& rhs) = delete;

	public:

		// Get if the given key is down
		bool IsKeyDown(eInputKey key) const;
		
		// Get if the given key is up
		bool IsKeyUp(eInputKey key) const;

		// Get if the given key was recently pressed
		bool WasKeyPressed(eInputKey key) const;

		// Get if the given key was recently released
		bool WasKeyReleased(eInputKey key) const;

		// Get the position of the mouse
		void GetMousePosition(int& x, int& y) const;
		void GetMousePosition(float& x, float& y) const;

		// Get the recent scroll offset of the mouse wheel
		float GetMouseScroll() const;

	private:

		// Initializes this input to track the input of the given window
		bool Initialize(Window* window);

		// Cleans up the association with the current window
		bool CleanUp();

		// Updates the polled inputs. This needs to be called
		// as certain events aren't continously polled
		void PollInputs();

		// Resets all input tracking
		void ClearRecords();

	private:

		// Event called by window when action has been performed on a key
		void HandleKeyEvent(eInputKey key, eInputAction action, eInputMods mods);

		// Event called by window when mouse has been moved
		void HandleMouseMove(double x, double y);

		// Event called by window when mouse wheel has been scrolled
		void HandleMouseScroll(double offset);

	private:

		// Window this input is associated with
		Window* m_Window;

		// Status of all keys (including mouse buttons)
		std::map<eInputKey, Key> m_KeyStatus;

		// Keys updated during last polling
		std::vector<eInputKey> m_UpdatedKeys;

		// Position of the mouse
		double m_MouseX, m_MouseY;

		// Scroll of the mouse wheel
		double m_MouseScroll;
	};
}