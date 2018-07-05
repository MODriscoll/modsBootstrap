#pragma once

#include "Types.h"

struct GLFWwindow;

namespace mods
{
	// TODO: appresult
	// returned by Run

	class Application
	{
	public:

		Application();
		virtual ~Application() = default;

	public:

		// Runs the application.
		// Returns result of execution
		int32 Run(int32 width, int32 height, const char* title);

	public:

		// Enable/Disables VSync
		void EnableVSync(bool enable) const;

	public:

		// Get windows size
		int32 GetWindowWidth() const;
		int32 GetWindowHeight() const;
		void GetWindowsSize(int32& width, int32& height) const;

		// Get time when application first started
		inline float GetStartTime() const { return m_AppStart; }

		// Get how long this application has been running for
		float GetExecutuinTime() const;

		// Get applications frames per second
		inline int32 GetFPS() const { return m_FPS; }

	protected:

		// Called before the game loop starts.
		// Returns if startup was successfull
		// virtual Result Startup() = 0
		virtual bool Startup() { return true; }

		// Called after the game loop has finished.
		// Returns if shutdown was successfull
		// virtual Result Shutdown() = 0
		virtual bool Shutdown() { return true; }
		
		// Called once per frame, should update the application
		virtual void Tick(float deltaTime) = 0;

		// Called once per frame after tick to draw the application to screen
		virtual void Draw() = 0;

	protected:

		// Stops the application loop
		void Stop();

	private:

		// Initializes the application
		bool Initialize(int32 width, int32 height, const char* title);

		// Cleansup the application
		bool Cleanup();

	private:

		// Handles errors encountered by glfw
		static void HandleGLFWError(int32 error, const char* message);

		// Handles resizing of window
		static void HandleFramebufferResize(GLFWwindow* window, int32 width, int32 height);

	private:

		// Window of the application
		GLFWwindow* m_Window;

		// Time the application started
		float m_AppStart;

		// Frames drawn during last second
		int32 m_FPS;
	};
}