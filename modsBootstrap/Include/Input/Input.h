#pragma once

#include "InputTypes.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace mods
{
	class mdWindow;

	// Handles the input for a window. Should
	// not be used outside of the window wrapper
	class mdInput final
	{
		friend mdWindow;

	private:

		mdInput(mdWindow* window);
		mdInput(const mdInput& rhs) = delete;
		mdInput(mdInput&& rhs) = delete;

		~mdInput();

		mdInput& operator = (const mdInput& rhs) = delete;
		mdInput& operator = (mdInput&& rhs) = delete;

	private:

		// Window this input is associated with
		mdWindow* m_Window;

		// Status of all keys associated keys
	};
}