#pragma once

#include "Texture.h"

#include <array>

namespace mods
{
	// Wrapper for a set of cubemap textures
	class Cubemap : public Texture
	{
	public:

		Cubemap() = default;
		Cubemap(const std::array<std::string, 6>& paths);
		Cubemap(const Cubemap& rhs) = delete;
		Cubemap(Cubemap&& rhs);

		virtual ~Cubemap() = default;

		Cubemap& operator = (const Cubemap& rhs) = delete;
		Cubemap& operator = (Cubemap&& rhs);

	public:

		// Creates a new cubemap texture from the given paths
		bool Load(const std::array<std::string, 6>& paths);

	protected:

		virtual uint32 GetTextureType() const override;
	};
}