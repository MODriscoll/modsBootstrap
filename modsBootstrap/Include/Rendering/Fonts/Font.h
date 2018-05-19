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

	// TODO: update to work using utf-8 strings, look
	// up better methods for rendering fonts, as this is very basic
	// To render a font correctly, blending must be enabled
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
			ShaderProgram& shader,
			const std::string& text,
			glm::vec2 position,
			const glm::vec4& color,
			float scale = 1.f) const;

	public:

		inline bool IsValid() const { return !m_Glyphs.empty(); }

		inline uint32 GetSize() const { return m_Size; }

	private:

		// Handle to quad vertex array object
		uint32 m_VBO, m_VAO;

		// All glyphs of this font
		std::map<byte, Glyph> m_Glyphs;

		// Size of this font
		uint32 m_Size;
	};
}