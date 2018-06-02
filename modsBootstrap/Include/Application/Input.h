#pragma once

#include "Types.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace mods
{
	enum class eInputKey : int32
	{
		Unknown				= -1,

		MouseLeft			= 0,
		MouseRight			= 1,
		MouseMiddle			= 2,

		Mouse4				= 3,
		Mouse5				= 4,
		Mouse6				= 5,
		Mouse7				= 6,
		Mouse8				= 7,

		//MouseX				= 10,
		//MouseY				= 11,

		Space				= 32,
		Apostrophe			= 39,
		Comma				= 44,
		Minus				= 45,
		Period				= 46,
		Slash				= 47,

		Zero				= 48,
		One					= 49,
		Two					= 50,
		Three				= 51,
		Four				= 52,
		Five				= 53,
		Six					= 54,
		Seven				= 55,
		Eight				= 56,
		Nine				= 57,

		SemiColon			= 59,
		Equal				= 61,

		A					= 65,
		B					= 66,
		C					= 67,
		D					= 68,
		E					= 69,
		F					= 70,
		G					= 71,
		H					= 72,
		I					= 73,
		J					= 74,
		K					= 75,
		L					= 76,
		M					= 77,
		N					= 78,
		O					= 79,
		P					= 80,
		Q					= 81,
		R					= 82,
		S					= 83,
		T					= 84,
		U					= 85,
		V					= 86,
		W					= 87,
		X					= 88,
		Y					= 89,
		Z					= 90,

		LeftBracket			= 91,
		BackSlash			= 92,
		RightBracket		= 93,
		GraveAccent			= 96,
		Escape				= 256,
		Enter				= 257,
		Tab					= 258,
		Backspace			= 259,
		Insert				= 260,
		Delete				= 261,
		Right				= 262,
		Left				= 263,
		Down				= 264,
		Up					= 265,
		PageUp				= 266,
		PageDown			= 267,
		Home				= 268,
		End					= 269,
		CapsLock			= 280,
		ScrollLock			= 281,
		NumLock				= 282,
		PrintScreen			= 283,
		Pause				= 284,

		F1					= 290,
		F2					= 291,
		F3					= 292,
		F4					= 293,
		F5					= 294,
		F6					= 295,
		F7					= 296,
		F8					= 297,
		F9					= 298,
		F10					= 299,
		F11					= 300,
		F12					= 301,
		F13					= 302,
		F14					= 303,
		F15					= 304,
		F16					= 305,
		F17					= 306,
		F18					= 307,
		F19					= 308,
		F20					= 309,
		F21					= 310,
		F22					= 311,
		F23					= 312,
		F24					= 313,
		F25					= 314,

		KP_0				= 320,
		KP_1				= 321,
		KP_2				= 322,
		KP_3				= 323,
		KP_4				= 324,
		KP_5				= 325,
		KP_6				= 326,
		KP_7				= 327,
		KP_8				= 328,
		KP_9				= 329,

		KP_Decimal			= 330,
		KP_Divide			= 331,
		KP_Multiply			= 332,
		KP_Subtract			= 333,
		KP_Add				= 334,
		KP_Enter			= 335,
		KP_Equal			= 336,

		LeftShift			= 340,
		LeftControl			= 341,
		LeftAlt				= 342,
		RightShift			= 344,
		RightControl		= 345,
		RightAlt			= 346
	};

	inline bool operator < (eInputKey lhs, eInputKey rhs)
	{
		return (static_cast<int32>(lhs) < static_cast<int32>(rhs));
	}

	enum class eInputAction : int32
	{
		Release				= 0,
		Press				= 1,
		Hold				= 2

		// TODO:
		//DoubleClick
	};

	// TODO: enum mod

	enum class eInputStatus : byte
	{
		Release		= 0,
		Press		= 1,

		Up			= 2,
		Down		= 3
	};

	struct InputBinding
	{
	public:

		InputBinding()
			: bConsume(false)
		{

		}

		virtual ~InputBinding() = default;

	public:

		// If this binding consumes the input
		bool bConsume;
	};

	struct KeyBinding : public InputBinding
	{
	public:

		KeyBinding(eInputKey key)
			: Key(key)
		{

		}

	public:

		// The key that is bound
		eInputKey Key;

		// All callbacks attached to this binding
		std::function<void()> Callbacks;
	};

	// First draft for input
	// Need to seperate so each window gets its own input context
	// Maybe have an input manager that has static functions similar to this
	// But returns content for current context window or requested window
	// Would like bindings to be hashed based
	// (each binding could have a name to query by, which should be hashed to increase performance)
	class Input
	{
		friend class Application;

	public:

		// Get status of the requested key
		static bool IsKeyDown(eInputKey key);
		static bool IsKeyUp(eInputKey key);

		// Get if a key was recently polled
		static bool WasKeyPressed(eInputKey key);
		static bool WasKeyReleased(eInputKey key);

		// Get mouse position
		static float GetMouseX();
		static float GetMouseY();
		static void GetMousePosition(float& x, float& y);

		// Get mouse delta 
		static float GetDeltaX();
		static float GetDeltaY();
		static void GetMouseDelta(float& x, float& y);

		// Get the mouse scroll
		static float GetMouseScroll();

	private:

		// Creates unique singleton instance
		static void Create();

		// Destroys unique singleton instance
		static void Destroy();

		// Updates status of inputs
		static void Update();

		// Get the singleton instance of the input
		// Asserts if input is not valid
		static Input& Instance();

		// Singleton instance
		static Input* m_Singleton;

	private:

		Input();
		~Input() = default;

		// Initializes input bindings
		void Initialize();

		// Cleans up bindings
		void Cleanup();

		// Executes any bindings and
		// updates status of keys
		void Poll();

		// Handles given key input
		void HandleKeyInputEvent(eInputKey key, eInputAction action);

		// Handles given mouse input
		void HandleMouseMoveEvent(double xpos, double ypos);

		// Handles given mouse scroll
		void HandleMouseScrollEvent(double yoffset);

		// Get the status of a key
		// Initializes key if it doesn't exist
		byte GetKeyStatus(eInputKey key);

	private:

		// Map of all keys status
		std::map<eInputKey, byte> m_Keys;

		// Keys that were polled last frame
		std::vector<eInputKey> m_PolledKeys;

		// Position of the mouse
		double m_MouseX, m_MouseY;

		// Position of mouse at end of last frame
		double m_OldMouseX, m_OldMouseY;

		// Scroll on the mouse wheel
		double m_MouseScroll;
	};
}