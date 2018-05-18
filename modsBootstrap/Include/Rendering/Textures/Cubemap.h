#pragma once

#include "Types.h"

#include <array>

namespace mods
{
	// TODO: set up similar to texture
	class Cubemap
	{
	public:

		Cubemap(const std::array<std::string, 6>& paths);
		Cubemap(const Cubemap& rhs) = delete;
		Cubemap(Cubemap&& rhs);

		~Cubemap();

		Cubemap& operator = (const Cubemap& rhs) = delete;
		Cubemap& operator = (Cubemap&& rhs);

	public:

		// Loads a new cubemap from the given paths.
		// Existing cubemap is destroyed
		bool Load(const std::array<std::string, 6>& paths);

		// Destroys and invalidates this cubemap
		bool Unload();

		// Binds this cubemap for use at the given slot
		void Bind(uint32 slot = 0);

		// Unbinds this cubemap from use
		void Unbind();

	public:

		inline bool IsValid() const { return m_Handle != 0; }

	private:

		// Handle to the cubemap
		uint32 m_Handle;
	};
}