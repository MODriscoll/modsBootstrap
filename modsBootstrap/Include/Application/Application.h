#pragma once

#include "Window\Window.h"

#include <chrono>

namespace mods
{
	using namespace std::chrono;

	// Base class for your application. This base handles
	// creating new windows and any errors thay may occur
	class Application
	{
	public:

		Application();
		virtual ~Application() = default;

	public:

		// Runs the application, should be called by main.
		// Returns if applications execution was successfull
		bool Run();

	protected:

		// Called to startup the application.
		// Returns if an error has occurred
		virtual bool Startup() = 0;

		// Called to shutdown the application.
		// Returns if an error has occured
		virtual bool Shutdown() = 0;

	protected:

		// Called once per frame to update the application
		virtual void Update(float DeltaTime) = 0;

	protected:

		// Default window of the application
		virtual Window* GetDefaultWindow() = 0;

		// Creates a new window of the specified type.
		// If the window creation failed, a null value is returned
		template <typename Type>
		Type* CreateNewWindow(const WindowContext& context, bool focus = true);

	protected:

		// Callback for when GLFW has encountered an error.
		// This function simply prints out the error provided
		virtual void HandleError(int error, const char* description);

	private:

		// Initializes the application
		bool Initialize();

		// Cleans up the application
		void Cleanup();

	protected:

		// Window with current focus
		Window* m_FocusedWindow;

		// All windows associated with this application
		std::vector<Window*> m_Windows;

	private:

		// Time when application was created
		high_resolution_clock::time_point m_AppStart;

		// Time when the last frame started
		high_resolution_clock::time_point m_FrameStart;

		// Delta time for the current frame
		float m_DeltaTime;
	};

	template<typename Type>
	inline Type* Application::CreateNewWindow(const WindowContext& context, bool focus)
	{
		static_assert(std::is_base_of<Window, Type>::value, "Type must be a window");

		return NULL;
	}
}