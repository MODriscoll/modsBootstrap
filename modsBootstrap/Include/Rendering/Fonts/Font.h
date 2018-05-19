#pragma once

#include "Types.h"

#include <glm\vec2.hpp>
#include <glm\vec4.hpp>

#include <map>
#include <string>

namespace mods
{
	class ShaderProgram;

	struct Glyph
	{
	public:

		Glyph()
			: Handle(0)
			, Size(0)
			, Bearing(0)
			, Offset(0)
		{

		}

	public:

		// Handle to this glyphs texture
		uint32 Handle;

		// Size of this glyph
		glm::ivec2 Size;

		// Bearing of this glyph
		glm::ivec2 Bearing;

		// Offset to the next glyph
		uint32 Offset;
	};

	class Font
	{
	public:

		Font(const std::string& path, uint32 size = 48);
		Font(const Font& rhs) = delete;
		Font(Font&& rhs);

		~Font();

		Font& operator = (const Font& rhs) = delete;
		Font& operator = (Font&& rhs);

	public:

		// Loads a new font from the given path.
		// Existing font is destroyed
		bool Load(const std::string& path, uint32 size = 48);

		// Destroys and invalidates this font
		bool Unload();

		// Draws the given text to screen
		void Draw(
			const std::string& text,
			const glm::vec2& position,
			const glm::vec4& color,
			float scale = 1.f);

	public:

		inline bool IsValid() const { return !m_Glyphs.empty(); }

		inline uint32 GetSize() const { return m_Size; }

	private:

		// All glyphs of this font
		std::map<byte, Glyph> m_Glyphs;

		// Size of this font
		uint32 m_Size;
	};
}